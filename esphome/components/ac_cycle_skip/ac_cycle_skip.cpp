#include "ac_cycle_skip.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include <cmath>

namespace esphome::ac_cycle_skip {

static const char *const TAG = "ac_cycle_skip";
static constexpr uint32_t Q16_FULL = 65535;
static ACCycleSkipDataStore *all_outputs[8];  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

uint32_t IRAM_ATTR HOT ACCycleSkipDataStore::update_target_(uint32_t now) {
  const uint32_t requested = this->requested_q16;

  if (requested == 0) {
    this->target_q16 = 0;
    this->boost_until_us = 0;
    return 0;
  }

  if (requested >= Q16_FULL) {
    this->target_q16 = Q16_FULL;
    this->boost_until_us = 0;
    return Q16_FULL;
  }

  if (this->boost_until_us != 0 && static_cast<int32_t>(this->boost_until_us - now) > 0) {
    this->target_q16 = Q16_FULL;
    return Q16_FULL;
  }
  this->boost_until_us = 0;

  uint32_t target = this->target_q16;
  if (this->ramp_ms == 0) {
    target = requested;
  } else if (target != requested) {
    const uint32_t ramp_us = this->ramp_ms * 1000UL;
    uint32_t step = (Q16_FULL * this->half_cycle_time_us) / ramp_us;
    if (step == 0)
      step = 1;

    if (target < requested) {
      const uint32_t delta = requested - target;
      target += delta < step ? delta : step;
    } else {
      const uint32_t delta = target - requested;
      target -= delta < step ? delta : step;
    }
  }

  this->target_q16 = target;
  return target;
}

void IRAM_ATTR HOT ACCycleSkipDataStore::gpio_intr() {
  const uint32_t now = micros();
  const uint32_t elapsed = now - this->crossed_zero_at;

  if (this->crossed_zero_at != 0 && elapsed < this->noise_filter_us) {
    this->rejected_crossings++;
    return;
  }

  if (this->crossed_zero_at != 0) {
    this->half_cycle_time_us = elapsed;
  }
  this->crossed_zero_at = now;

  const uint32_t target = this->update_target_(now);
  if (target == 0) {
    this->gate_pin.digital_write(false);
    this->accumulator_q16 = 0;
    this->second_half_cycle = false;
    this->cycle_on = false;
    return;
  }

  if (target >= Q16_FULL) {
    this->gate_pin.digital_write(true);
    this->second_half_cycle = !this->second_half_cycle;
    this->cycle_on = true;
    return;
  }

  if (!this->second_half_cycle) {
    this->accumulator_q16 += target;
    if (this->accumulator_q16 >= Q16_FULL) {
      this->accumulator_q16 -= Q16_FULL;
      this->cycle_on = true;
    } else {
      this->cycle_on = false;
    }
  }

  this->gate_pin.digital_write(this->cycle_on);
  this->second_half_cycle = !this->second_half_cycle;
}

void IRAM_ATTR HOT ACCycleSkipDataStore::s_gpio_intr(ACCycleSkipDataStore *store) {
  for (auto *output : all_outputs) {
    if (output == nullptr)
      break;
    if (output->zero_cross_pin_number == store->zero_cross_pin_number) {
      output->gpio_intr();
    }
  }
}

void ACCycleSkipOutput::setup() {
  bool setup_zero_cross_pin = true;

  for (auto &output : all_outputs) {
    if (output == nullptr) {
      output = &this->store_;
      break;
    }
    if (output->zero_cross_pin_number == this->zero_cross_pin_->get_pin()) {
      setup_zero_cross_pin = false;
    }
  }

  this->gate_pin_->setup();
  this->gate_pin_->digital_write(false);
  this->store_.gate_pin = this->gate_pin_->to_isr();
  this->store_.zero_cross_pin_number = this->zero_cross_pin_->get_pin();

  if (setup_zero_cross_pin) {
    this->zero_cross_pin_->setup();
    this->store_.zero_cross_pin = this->zero_cross_pin_->to_isr();
    this->zero_cross_pin_->attach_interrupt(&ACCycleSkipDataStore::s_gpio_intr, &this->store_,
                                            this->zero_cross_interrupt_type_);
  }
}

void ACCycleSkipOutput::write_state(float state) {
  if (state <= 0.0f || std::isnan(state)) {
    this->store_.requested_q16 = 0;
    this->store_.target_q16 = 0;
    this->store_.boost_until_us = 0;
    return;
  }

  uint32_t requested = Q16_FULL;
  if (state >= 1.0f) {
    requested = Q16_FULL;
  } else {
    requested = static_cast<uint32_t>(state * Q16_FULL + 0.5f);
  }

  const bool starting = this->store_.requested_q16 == 0 && requested > 0 && requested < Q16_FULL;
  this->store_.requested_q16 = requested;
  if (starting && this->store_.start_boost_ms > 0) {
    this->store_.target_q16 = Q16_FULL;
    this->store_.boost_until_us = micros() + this->store_.start_boost_ms * 1000UL;
  }
}

void ACCycleSkipOutput::dump_config() {
  ESP_LOGCONFIG(TAG, "AC Cycle Skip Output:");
  LOG_PIN("  Gate Pin: ", this->gate_pin_);
  LOG_PIN("  Zero-Cross Pin: ", this->zero_cross_pin_);
  ESP_LOGCONFIG(TAG,
                "  Noise filter: %uus\n"
                "  Start boost: %ums\n"
                "  Ramp time: %ums\n"
                "  Estimated mains: %.2f Hz\n"
                "  Rejected zero-cross pulses: %u",
                this->store_.noise_filter_us, this->store_.start_boost_ms, this->store_.ramp_ms,
                1e6f / this->store_.half_cycle_time_us / 2.0f, this->store_.rejected_crossings);
  LOG_FLOAT_OUTPUT(this);
}

}  // namespace esphome::ac_cycle_skip
