#include <cmath>

#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "xdb401.h"

namespace esphome::xdb401 {

static const char *const TAG = "xdb401";

static constexpr uint8_t REG_DATA = 0x06;
static constexpr uint8_t REG_MAKE_MEASURE = 0x30;
static constexpr uint8_t CMD_MAKE_MEASURE = 0x0A;
static constexpr uint8_t MASK_MEASURE_BUSY = 0x08;
static constexpr float CONVERT_PRESSURE = 8388608.0f;  // 0x800000
static constexpr uint32_t READY_CHECK_DELAY_MS = 5;
static constexpr uint8_t READY_CHECK_ATTEMPTS = 20;
static constexpr float MIN_VALID_TEMPERATURE_C = -20.0f;
static constexpr float MAX_VALID_TEMPERATURE_C = 100.0f;
static constexpr float MAX_TEMPERATURE_STEP_C = 5.0f;

void XDB401Component::setup() {
  ESP_LOGCONFIG(TAG, "Running setup (local reliable-read patch)");

  uint8_t meas_resp[1]{};
  const i2c::ErrorCode err_code = this->read_register(REG_MAKE_MEASURE, meas_resp, sizeof(meas_resp));
  if (err_code != i2c::ERROR_OK) {
    this->mark_failed(LOG_STR("I2C communication failed"));
    return;
  }
  this->comm_err_counter_ = 0;
  this->publish_diagnostics_();
  if (this->last_error_sensor_ != nullptr)
    this->last_error_sensor_->publish_state("none");
}

void XDB401Component::dump_config() {
  ESP_LOGCONFIG(TAG, "XDB401 (local reliable-read patch):");
  LOG_I2C_DEVICE(this);
  LOG_UPDATE_INTERVAL(this);
  ESP_LOGCONFIG(TAG, "  Pressure Range: %u bar", this->pressure_range_bar_);
  ESP_LOGCONFIG(TAG, "  Ready check: every %u ms, up to %u ms", READY_CHECK_DELAY_MS,
                READY_CHECK_DELAY_MS * READY_CHECK_ATTEMPTS);
  LOG_SENSOR("  ", "Pressure", this->pressure_sensor_);
  LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
}

void XDB401Component::publish_diagnostics_() {
  if (this->start_error_sensor_ != nullptr)
    this->start_error_sensor_->publish_state(this->start_error_count_);
  if (this->status_error_sensor_ != nullptr)
    this->status_error_sensor_->publish_state(this->status_error_count_);
  if (this->measurement_timeout_sensor_ != nullptr)
    this->measurement_timeout_sensor_->publish_state(this->measurement_timeout_count_);
  if (this->packet_error_sensor_ != nullptr)
    this->packet_error_sensor_->publish_state(this->packet_error_count_);
  if (this->rejected_packet_sensor_ != nullptr)
    this->rejected_packet_sensor_->publish_state(this->rejected_packet_count_);
  if (this->total_error_sensor_ != nullptr)
    this->total_error_sensor_->publish_state(this->total_error_count_);
  if (this->consecutive_error_sensor_ != nullptr)
    this->consecutive_error_sensor_->publish_state(this->consecutive_error_count_);
}

void XDB401Component::handle_comm_failure_(ErrorType type, const char *message) {
  this->status_set_warning(message);
  // Runtime I2C errors must never disable polling permanently. Keep retrying on
  // subsequent update cycles so a transient bus error can recover by itself.
  if (this->comm_err_counter_ < 0xFF) {
    this->comm_err_counter_++;
  }
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
    case ErrorType::REJECTED_PACKET:
      this->rejected_packet_count_++;
      break;
  }
  this->total_error_count_++;
  this->consecutive_error_count_++;
  this->publish_diagnostics_();
  if (this->last_error_sensor_ != nullptr)
    this->last_error_sensor_->publish_state(message);
  this->measurement_in_progress_ = false;
}

i2c::ErrorCode XDB401Component::start_measurement_() {
  return this->write_register(REG_MAKE_MEASURE, &CMD_MAKE_MEASURE, sizeof(CMD_MAKE_MEASURE));
}

void XDB401Component::check_measurement_ready_(uint8_t attempt) {
  uint8_t status[1]{};
  const i2c::ErrorCode err_code = this->read_register(REG_MAKE_MEASURE, status, sizeof(status));
  if (err_code != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Could not read measurement status, code: %u", err_code);
    this->handle_comm_failure_(ErrorType::STATUS, "status_read_error");
    return;
  }

  // Bit 3 clears when the pressure and temperature packet is ready.
  if ((status[0] & MASK_MEASURE_BUSY) == 0) {
    this->read_measurement_();
    return;
  }

  if (attempt >= READY_CHECK_ATTEMPTS) {
    ESP_LOGW(TAG, "Measurement was not ready after %lu ms",
             static_cast<unsigned long>(READY_CHECK_DELAY_MS * READY_CHECK_ATTEMPTS));
    this->handle_comm_failure_(ErrorType::TIMEOUT, "measurement_timeout");
    return;
  }

  this->set_timeout(READY_CHECK_DELAY_MS,
                    [this, attempt]() { this->check_measurement_ready_(attempt + 1); });
}

void XDB401Component::read_measurement_() {
  // Read pressure (3 bytes) and temperature (2 bytes) in one coherent I2C transaction.
  uint8_t data[5]{};
  const i2c::ErrorCode err_code = this->read_register(REG_DATA, data, sizeof(data));
  if (err_code != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Could not read complete pressure/temperature packet, code: %u", err_code);
    this->handle_comm_failure_(ErrorType::PACKET, "packet_read_error");
    return;
  }

  const int32_t raw_pressure = static_cast<int32_t>(encode_uint24(data[0], data[1], data[2]) << 8) >> 8;
  const int16_t raw_temperature = static_cast<int16_t>(encode_uint16(data[3], data[4]));
  const float pressure = (static_cast<float>(raw_pressure) / CONVERT_PRESSURE) *
                         XDB401Component::full_scale_pressure_pa(this->pressure_range_bar_);
  const float temperature = static_cast<float>(raw_temperature) / 256.0f;

  const bool temperature_in_range =
      std::isfinite(temperature) && temperature >= MIN_VALID_TEMPERATURE_C && temperature <= MAX_VALID_TEMPERATURE_C;
  const bool temperature_step_ok =
      !this->have_last_temperature_ || std::fabs(temperature - this->last_temperature_) <= MAX_TEMPERATURE_STEP_C;
  if (!std::isfinite(pressure) || !temperature_in_range || !temperature_step_ok) {
    ESP_LOGW(TAG, "Rejected implausible packet: pressure=%.1f Pa, temperature=%.2f C", pressure, temperature);
    this->handle_comm_failure_(ErrorType::REJECTED_PACKET, "rejected_packet");
    return;
  }

  ESP_LOGD(TAG, "Got pressure=%.1f Pa, temperature=%.2f C", pressure, temperature);
  this->last_temperature_ = temperature;
  this->have_last_temperature_ = true;

  if (this->temperature_sensor_ != nullptr)
    this->temperature_sensor_->publish_state(temperature);
  if (this->pressure_sensor_ != nullptr)
    this->pressure_sensor_->publish_state(pressure);

  this->comm_err_counter_ = 0;
  if (this->consecutive_error_count_ != 0) {
    this->consecutive_error_count_ = 0;
    if (this->consecutive_error_sensor_ != nullptr)
      this->consecutive_error_sensor_->publish_state(0);
  }
  this->status_clear_warning();
  this->measurement_in_progress_ = false;
}

void XDB401Component::update() {
  if (this->measurement_in_progress_) {
    ESP_LOGV(TAG, "Skipping update, measurement already in progress");
    return;
  }

  const i2c::ErrorCode err_code = this->start_measurement_();
  if (err_code != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Error starting measurement, code: %u", err_code);
    this->handle_comm_failure_(ErrorType::START, "start_measurement_error");
    return;
  }

  this->measurement_in_progress_ = true;
  this->set_timeout(READY_CHECK_DELAY_MS, [this]() { this->check_measurement_ready_(1); });
}

}  // namespace esphome::xdb401
