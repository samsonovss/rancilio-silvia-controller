#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>

#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "xdb401.h"

namespace esphome::xdb401 {

static const char *const TAG = "xdb401";

static constexpr uint8_t REG_DATA = 0x06;
static constexpr uint8_t REG_MAKE_MEASURE = 0x30;
static constexpr uint8_t CMD_MAKE_MEASURE = 0x0A;

// Match the XIDIBEI/mp-se acquisition sequence:
// start conversion -> fixed delay -> select 0x06 with STOP -> separate 5-byte read.
static constexpr uint32_t CONVERSION_DELAY_MS = 50;

static constexpr float CONVERT_PRESSURE = 8388608.0f;  // 0x800000

// Basic sanity limits.
static constexpr float MIN_VALID_TEMPERATURE_C = -20.0f;
static constexpr float MAX_VALID_TEMPERATURE_C = 100.0f;
static constexpr float MAX_TEMPERATURE_STEP_C = 5.0f;
static constexpr float MIN_VALID_PRESSURE_PA = -50000.0f;  // -0.5 bar
static constexpr float MAX_VALID_PRESSURE_FACTOR = 1.03f;

// Pressure jump validation.
//
// The old implementation used a fixed 1.2 bar limit forever.
// After one missed packet that could permanently lock the driver
// on an old pressure value.
//
// Now the permitted change grows with the age of the last accepted pressure.
//
// The slew allowance was measured from recorded shots: after the EMA filter
// the pressure ramp peaks around 10 bar/s, and the EMA (alpha 0.35) damps the
// raw transient roughly threefold, so the real sensor slew reaches ~15 bar/s
// when the pump kicks in against a full basket. The previous 6.5 bar/s limit
// rejected legitimate packets on every ramp, which starved the YAML fail-safe
// (sensor_age <= 350 ms) and cut the pump mid-shot.
//
// The allowance is asymmetric: see the rise/fall reasoning at the step check.
static constexpr float BASE_MAX_PRESSURE_STEP_PA = 120000.0f;          // 1.2 bar
static constexpr float MAX_PRESSURE_RISE_RATE_PA_PER_S = 1500000.0f;  // 15 bar/s
static constexpr float MAX_PRESSURE_FALL_RATE_PA_PER_S = 4000000.0f;  // 40 bar/s

// Fixed allowance on top of the rate term, sized for the pump's own ripple.
//
// Silvia uses a vibratory pump: the water column pulsates at mains frequency
// while the sensor is polled at 10 Hz, so every sample lands on a different
// phase of the pump stroke. Measured shots show ~0.5 bar of sample-to-sample
// ripple that never averages out. On a flat plateau it is invisible, but on a
// ramp it adds to the trend and a legitimate reading gets refused.
//
// This term is deliberately a constant, not a higher rise rate: it does not
// grow with the gap length, so it covers single-sample ripple without opening
// a hole after a long stall, where a genuinely bogus jump must still be caught.
static constexpr float PRESSURE_STEP_MARGIN_PA = 80000.0f;            // 0.8 bar

// NOTE: the full-scale "rail" rejection was removed.
//
// It assumed the sensor span is far above the machine's working range. This
// build uses a 12 bar sensor on a machine whose OPV opens around 10-12 bar, so
// full scale IS a reachable working pressure. Worse, the check compared against
// last_pressure_, which is never updated while a packet is rejected: one real
// over-pressure event would have latched the reading forever until reboot.
// Sudden bogus jumps are still caught by the dynamic step check below.

// Project currently uses one XDB401 instance.
// Keeping this timestamp here means xdb401.h does not need modification.
static uint32_t last_accepted_pressure_ms = 0;

void XDB401Component::setup() {
  ESP_LOGCONFIG(TAG, "Running setup (fixed-delay / STOP-separated XDB401 read)");

  this->measurement_in_progress_ = false;
  last_accepted_pressure_ms = 0;

  this->publish_diagnostics_();
  this->publish_health_();

  if (this->last_error_sensor_ != nullptr)
    this->last_error_sensor_->publish_state("none");

  if (this->last_rejection_sensor_ != nullptr)
    this->last_rejection_sensor_->publish_state("нет");
}

// Single readable answer to "is this a physical sensor/bus problem?".
//
// Only link faults can mean broken hardware. Rejections are our own filter
// refusing a value that arrived perfectly well, so they must never colour the
// verdict - that confusion is exactly what this sensor exists to remove.
void XDB401Component::publish_health_() {
  const char *health;

  if (this->consecutive_error_count_ >= 2) {
    health = "Нет связи с датчиком";
  } else if (this->consecutive_error_count_ > 0) {
    health = "Сбой связи, идёт восстановление";
  } else if (this->link_error_count_ > 0) {
    health = "Исправен, были сбои связи";
  } else {
    health = "Исправен";
  }

  if (this->published_health_ != nullptr && std::strcmp(this->published_health_, health) == 0)
    return;

  this->published_health_ = health;

  if (this->health_sensor_ != nullptr)
    this->health_sensor_->publish_state(health);
}

void XDB401Component::dump_config() {
  ESP_LOGCONFIG(TAG, "XDB401:");
  LOG_I2C_DEVICE(this);
  LOG_UPDATE_INTERVAL(this);

  ESP_LOGCONFIG(TAG, "  Pressure Range: %u bar", this->pressure_range_bar_);
  ESP_LOGCONFIG(TAG, "  Conversion delay: %lu ms",
                static_cast<unsigned long>(CONVERSION_DELAY_MS));
  ESP_LOGCONFIG(TAG, "  Status polling: disabled");
  ESP_LOGCONFIG(TAG,
                "  Data read: write 0x06 + STOP, then separate 5-byte read");

  ESP_LOGCONFIG(TAG, "  Base pressure-step limit: %.2f bar",
                BASE_MAX_PRESSURE_STEP_PA / 100000.0f);

  ESP_LOGCONFIG(TAG, "  Pressure slew allowance: rise %.2f bar/s, fall %.2f bar/s",
                MAX_PRESSURE_RISE_RATE_PA_PER_S / 100000.0f,
                MAX_PRESSURE_FALL_RATE_PA_PER_S / 100000.0f);

  ESP_LOGCONFIG(TAG, "  Ripple margin: %.2f bar", PRESSURE_STEP_MARGIN_PA / 100000.0f);
  ESP_LOGCONFIG(TAG, "  Rejections raise consecutive errors: no");

  LOG_SENSOR("  ", "Pressure", this->pressure_sensor_);
  LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
}

void XDB401Component::publish_diagnostics_() {
  if (this->start_error_sensor_ != nullptr)
    this->start_error_sensor_->publish_state(this->start_error_count_);

  if (this->status_error_sensor_ != nullptr)
    this->status_error_sensor_->publish_state(this->status_error_count_);

  if (this->measurement_timeout_sensor_ != nullptr)
    this->measurement_timeout_sensor_->publish_state(
        this->measurement_timeout_count_);

  if (this->packet_error_sensor_ != nullptr)
    this->packet_error_sensor_->publish_state(this->packet_error_count_);

  if (this->rejected_packet_sensor_ != nullptr)
    this->rejected_packet_sensor_->publish_state(
        this->rejected_packet_count_);

  if (this->total_error_sensor_ != nullptr)
    this->total_error_sensor_->publish_state(this->total_error_count_);

  if (this->consecutive_error_sensor_ != nullptr)
    this->consecutive_error_sensor_->publish_state(
        this->consecutive_error_count_);

  if (this->link_error_sensor_ != nullptr)
    this->link_error_sensor_->publish_state(this->link_error_count_);

  if (this->pressure_rejected_sensor_ != nullptr)
    this->pressure_rejected_sensor_->publish_state(
        this->pressure_rejected_count_);

  if (this->temperature_rejected_sensor_ != nullptr)
    this->temperature_rejected_sensor_->publish_state(
        this->temperature_rejected_count_);
}

void XDB401Component::handle_comm_failure_(ErrorType type,
                                           const char *message) {
  const bool link_fault = is_link_fault(type);

  switch (type) {
    case ErrorType::START:
      this->start_error_count_++;
      break;

    case ErrorType::STATUS:
      this->status_error_count_++;
      break;

    case ErrorType::TIMEOUT:
      this->measurement_timeout_count_++;
      break;

    case ErrorType::PACKET:
      this->packet_error_count_++;
      break;

    case ErrorType::PRESSURE_REJECTED:
      this->pressure_rejected_count_++;
      this->rejected_packet_count_++;
      break;

    case ErrorType::TEMPERATURE_REJECTED:
      this->temperature_rejected_count_++;
      this->rejected_packet_count_++;
      break;
  }

  this->total_error_count_++;

  if (link_fault) {
    this->link_error_count_++;

    // Only a failed transfer may raise the consecutive counter, because the
    // YAML pump guard cuts drive at consecutive_errors >= 2.
    //
    // A rejection used to raise it too, which meant two ripple-induced
    // rejections in a row could stop the pump mid-shot with a perfectly
    // healthy sensor. Staleness is still covered: while packets are rejected
    // the published pressure stops updating, so sensor_age crosses its 350 ms
    // limit and the same guard fires - on evidence instead of on a guess.
    this->consecutive_error_count_++;

    this->status_set_warning(message);

    if (this->last_error_sensor_ != nullptr)
      this->last_error_sensor_->publish_state(message);
  } else if (this->last_rejection_sensor_ != nullptr) {
    // Sticky on purpose: last_error_ returns to "none" within ~100 ms, which
    // made the actual reason unreadable in any UI.
    this->last_rejection_sensor_->publish_state(message);
  }

  this->publish_diagnostics_();
  this->publish_health_();

  // Runtime failures must never permanently disable polling.
  this->measurement_in_progress_ = false;
}

i2c::ErrorCode XDB401Component::start_measurement_() {
  // Arduino Wire equivalent:
  //
  // beginTransmission(addr);
  // write(0x30);
  // write(0x0A);
  // endTransmission();

  return this->write_register(
      REG_MAKE_MEASURE,
      &CMD_MAKE_MEASURE,
      sizeof(CMD_MAKE_MEASURE));
}

// Kept because the current header declares it.
// This build intentionally does not poll a ready/status register.
void XDB401Component::check_measurement_ready_(uint8_t attempt) {
  (void) attempt;
  this->read_measurement_();
}

void XDB401Component::read_measurement_() {
  // Reference transaction sequence:
  //
  // write 0x06 + STOP
  // separate 5-byte read
  //
  // Do not replace this with read_register() in this test build.

  const uint8_t data_register = REG_DATA;

  i2c::ErrorCode err_code =
      this->write(&data_register, sizeof(data_register));

  if (err_code != i2c::ERROR_OK) {
    ESP_LOGW(TAG,
             "Could not select data register 0x%02X, code: %u",
             REG_DATA,
             err_code);

    this->handle_comm_failure_(
        ErrorType::PACKET,
        "packet_register_select_error");

    return;
  }

  uint8_t data[5]{};

  err_code = this->read(data, sizeof(data));

  if (err_code != i2c::ERROR_OK) {
    ESP_LOGW(TAG,
             "Could not read complete pressure/temperature packet, code: %u",
             err_code);

    this->handle_comm_failure_(
        ErrorType::PACKET,
        "packet_read_error");

    return;
  }

  // -------------------------------------------------------------------------
  // Decode raw packet
  // -------------------------------------------------------------------------

  const int32_t raw_pressure =
      static_cast<int32_t>(
          encode_uint24(data[0], data[1], data[2]) << 8) >>
      8;

  const int16_t raw_temperature =
      static_cast<int16_t>(
          encode_uint16(data[3], data[4]));

  const float full_scale_pa =
      XDB401Component::full_scale_pressure_pa(
          this->pressure_range_bar_);

  const float pressure =
      (static_cast<float>(raw_pressure) / CONVERT_PRESSURE) *
      full_scale_pa;

  const float temperature =
      static_cast<float>(raw_temperature) / 256.0f;

  // Raw packet trace.
  // Normal packets are VERBOSE.
  // Rejected packets are printed again as WARNING below.
  ESP_LOGV(
      TAG,
      "RAW=%02X %02X %02X %02X %02X "
      "rawP=%ld rawT=%d P=%.3f bar T=%.2f C",
      data[0],
      data[1],
      data[2],
      data[3],
      data[4],
      static_cast<long>(raw_pressure),
      static_cast<int>(raw_temperature),
      pressure / 100000.0f,
      temperature);

  // =========================================================================
  // PRESSURE VALIDATION
  // =========================================================================

  const float max_valid_pressure =
      full_scale_pa * MAX_VALID_PRESSURE_FACTOR;

  const bool pressure_in_range =
      std::isfinite(pressure) &&
      pressure >= MIN_VALID_PRESSURE_PA &&
      pressure <= max_valid_pressure;

  if (!pressure_in_range) {
    ESP_LOGW(
        TAG,
        "Rejected pressure range: "
        "RAW=%02X %02X %02X %02X %02X "
        "rawP=%ld P=%.3f bar T=%.2f C",
        data[0],
        data[1],
        data[2],
        data[3],
        data[4],
        static_cast<long>(raw_pressure),
        pressure / 100000.0f,
        temperature);

    this->handle_comm_failure_(
        ErrorType::PRESSURE_REJECTED,
        "Давление вне диапазона");

    return;
  }

  // -------------------------------------------------------------------------
  // Dynamic pressure step validation
  // -------------------------------------------------------------------------
  //
  // OLD:
  //
  // abs(new - old) > 1.2 bar
  //     -> reject forever
  //
  // NEW:
  //
  // permitted delta grows according to how much time has passed since the
  // last accepted packet.
  //
  // Example:
  //
  // normal packet timing:
  //     strict protection remains active
  //
  // packets lost for 600 ms:
  //     allowed difference increases
  //
  // therefore a healthy sensor can catch up with the real pressure.

  if (this->have_last_pressure_) {
    const uint32_t now = millis();

    // If timestamp has not yet been established use the expected
    // approximate sensor update interval.
    uint32_t elapsed_ms = 100;

    if (last_accepted_pressure_ms != 0)
      elapsed_ms = now - last_accepted_pressure_ms;

    if (elapsed_ms < 100)
      elapsed_ms = 100;

    const float elapsed_s =
        static_cast<float>(elapsed_ms) / 1000.0f;

    // The physics is not symmetric, so the limit must not be either.
    //
    // Rising pressure is bounded by how fast the pump can build it against
    // the puck. Falling pressure is not: the three-way valve dumps the group
    // almost instantly, and the same happens whenever the pump stops.
    //
    // A falsely LOW reading is also the safe failure direction for pump
    // control - it pushes the controller towards stopping. A falsely HIGH
    // reading is the dangerous one. So keep the rise limit tight and let
    // drops through.
    const float signed_step_pa =
        pressure - this->last_pressure_;

    const bool rising = signed_step_pa >= 0.0f;

    const float rate_limit_pa_per_s =
        rising ? MAX_PRESSURE_RISE_RATE_PA_PER_S
               : MAX_PRESSURE_FALL_RATE_PA_PER_S;

    const float rate_allowance =
        rate_limit_pa_per_s * elapsed_s +
        PRESSURE_STEP_MARGIN_PA;

    const float allowed_step_pa =
        std::max(
            BASE_MAX_PRESSURE_STEP_PA,
            rate_allowance);

    const float actual_step_pa =
        std::fabs(signed_step_pa);

    if (actual_step_pa > allowed_step_pa) {
      ESP_LOGW(
          TAG,
          "Rejected pressure step (%s): "
          "last=%.3f bar candidate=%.3f bar "
          "delta=%.3f bar allowed=%.3f bar "
          "age=%lu ms "
          "RAW=%02X %02X %02X %02X %02X",
          rising ? "rise" : "fall",
          this->last_pressure_ / 100000.0f,
          pressure / 100000.0f,
          actual_step_pa / 100000.0f,
          allowed_step_pa / 100000.0f,
          static_cast<unsigned long>(elapsed_ms),
          data[0],
          data[1],
          data[2],
          data[3],
          data[4]);

      this->handle_comm_failure_(
          ErrorType::PRESSURE_REJECTED,
          "Слишком резкий скачок давления");

      return;
    }
  }

  // =========================================================================
  // PRESSURE ACCEPTED
  // =========================================================================

  this->last_pressure_ = pressure;
  this->have_last_pressure_ = true;

  last_accepted_pressure_ms = millis();

  if (this->pressure_sensor_ != nullptr)
    this->pressure_sensor_->publish_state(pressure);

  // =========================================================================
  // TEMPERATURE VALIDATION
  // =========================================================================
  //
  // Important change:
  //
  // BAD TEMPERATURE MUST NOT BLOCK GOOD PRESSURE.
  //
  // Pressure is the value used to control the pump.
  // Temperature from XDB401 is only additional sensor information.

  const bool temperature_in_range =
      std::isfinite(temperature) &&
      temperature >= MIN_VALID_TEMPERATURE_C &&
      temperature <= MAX_VALID_TEMPERATURE_C;

  const bool temperature_step_ok =
      !this->have_last_temperature_ ||
      std::fabs(
          temperature - this->last_temperature_) <=
          MAX_TEMPERATURE_STEP_C;

  bool temperature_valid = true;
  const char *temperature_error = nullptr;

  if (!temperature_in_range) {
    temperature_valid = false;
    temperature_error =
        "Температура вне диапазона";

    ESP_LOGW(
        TAG,
        "Temperature rejected by range check: "
        "T=%.2f C "
        "RAW=%02X %02X %02X %02X %02X",
        temperature,
        data[0],
        data[1],
        data[2],
        data[3],
        data[4]);

  } else if (!temperature_step_ok) {
    temperature_valid = false;
    temperature_error =
        "Слишком резкий скачок температуры";

    ESP_LOGW(
        TAG,
        "Temperature rejected by step check: "
        "last=%.2f C candidate=%.2f C "
        "RAW=%02X %02X %02X %02X %02X",
        this->last_temperature_,
        temperature,
        data[0],
        data[1],
        data[2],
        data[3],
        data[4]);
  }

  if (temperature_valid) {
    this->last_temperature_ = temperature;
    this->have_last_temperature_ = true;

    if (this->temperature_sensor_ != nullptr)
      this->temperature_sensor_->publish_state(
          temperature);

  } else {
    // Temperature validation failure is recorded for diagnostics only. It does
    // not invalidate the already accepted pressure and, like every rejection,
    // it does not touch the consecutive counter that guards the pump.
    this->handle_comm_failure_(
        ErrorType::TEMPERATURE_REJECTED,
        temperature_error);
  }

  // A valid pressure packet means the link recovered.
  if (this->consecutive_error_count_ != 0) {
    this->consecutive_error_count_ = 0;

    if (this->consecutive_error_sensor_ != nullptr)
      this->consecutive_error_sensor_->publish_state(0);

    this->publish_health_();
  }

  this->status_clear_warning();

  if (this->last_error_sensor_ != nullptr)
    this->last_error_sensor_->publish_state("none");

  if (temperature_valid) {
    ESP_LOGD(
        TAG,
        "Got pressure=%.3f bar, temperature=%.2f C",
        pressure / 100000.0f,
        temperature);
  } else {
    ESP_LOGD(
        TAG,
        "Got pressure=%.3f bar, temperature=REJECTED",
        pressure / 100000.0f);
  }

  this->measurement_in_progress_ = false;
}

void XDB401Component::update() {
  if (this->measurement_in_progress_) {
    ESP_LOGV(
        TAG,
        "Skipping update, measurement already in progress");
    return;
  }

  const i2c::ErrorCode err_code =
      this->start_measurement_();

  if (err_code != i2c::ERROR_OK) {
    ESP_LOGW(
        TAG,
        "Error starting measurement, code: %u",
        err_code);

    this->handle_comm_failure_(
        ErrorType::START,
        "start_measurement_error");

    return;
  }

  this->measurement_in_progress_ = true;

  // Same fixed-delay reference path.
  // No status polling.
  this->set_timeout(
      CONVERSION_DELAY_MS,
      [this]() {
        this->read_measurement_();
      });
}

}  // namespace esphome::xdb401