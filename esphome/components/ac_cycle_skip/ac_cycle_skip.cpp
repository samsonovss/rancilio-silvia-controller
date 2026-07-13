#include "ac_cycle_skip.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include <cmath>
#include <cstdint>
#include <esp_err.h>

namespace esphome::ac_cycle_skip {

static const char *const TAG = "ac_cycle_skip";
static constexpr uint32_t Q16_FULL = 65535;
static ACCycleSkipDataStore *all_outputs[8];  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

void IRAM_ATTR HOT ACCycleSkipDataStore::force_off_() {
  this->pulse_generation.fetch_add(1, std::memory_order_relaxed);
  this->scheduled_pulse_generation.store(0, std::memory_order_relaxed);
  if (this->gate_timer != nullptr) {
    gptimer_set_alarm_action(this->gate_timer, nullptr);
  }
  this->gate_timer_phase.store(ACCycleSkipGateTimerPhase::IDLE, std::memory_order_relaxed);
  this->gate_pin.digital_write(false);
  this->cycle_on = false;
}

void IRAM_ATTR HOT ACCycleSkipDataStore::schedule_gate_pulse_() {
  if (this->gate_timer == nullptr) {
    this->force_off_();
    return;
  }

  uint64_t now = 0;
  if (gptimer_get_raw_count(this->gate_timer, &now) != ESP_OK) {
    this->force_off_();
    return;
  }

  this->gate_pin.digital_write(false);
  const uint32_t generation = this->pulse_generation.fetch_add(1, std::memory_order_relaxed) + 1;
  this->scheduled_pulse_generation.store(generation, std::memory_order_relaxed);
  this->gate_timer_phase.store(ACCycleSkipGateTimerPhase::WAITING_GATE_ON, std::memory_order_relaxed);

  gptimer_alarm_config_t alarm_config = {};
  alarm_config.alarm_count = now + this->gate_delay_us.load(std::memory_order_relaxed);
  alarm_config.flags.auto_reload_on_alarm = false;
  if (gptimer_set_alarm_action(this->gate_timer, &alarm_config) != ESP_OK) {
    this->force_off_();
  }
}

void IRAM_ATTR HOT ACCycleSkipDataStore::reset_sync_(uint32_t now) {
  this->force_off_();
  this->crossed_zero_at = now;
  this->valid_sync_intervals = 0;
  this->synchronized = false;
  this->second_half_cycle = false;
  this->accumulator_q16 = 0;
}

uint32_t IRAM_ATTR HOT ACCycleSkipDataStore::update_target_(uint32_t now) {
  const uint32_t requested = this->requested_q16.load(std::memory_order_relaxed);

  if (requested == 0) {
    this->target_q16.store(0, std::memory_order_relaxed);
    this->boost_until_us.store(0, std::memory_order_relaxed);
    return 0;
  }

  if (requested >= Q16_FULL) {
    this->target_q16.store(Q16_FULL, std::memory_order_relaxed);
    this->boost_until_us.store(0, std::memory_order_relaxed);
    return Q16_FULL;
  }

  const uint32_t boost_until = this->boost_until_us.load(std::memory_order_relaxed);
  if (boost_until != 0 && static_cast<int32_t>(boost_until - now) > 0) {
    this->target_q16.store(Q16_FULL, std::memory_order_relaxed);
    return Q16_FULL;
  }
  this->boost_until_us.store(0, std::memory_order_relaxed);

  uint32_t target = this->target_q16.load(std::memory_order_relaxed);
  const uint32_t ramp_ms = this->ramp_ms.load(std::memory_order_relaxed);
  if (ramp_ms == 0) {
    target = requested;
  } else if (target != requested) {
    const uint32_t ramp_us = ramp_ms * 1000UL;
    uint32_t step = (uint64_t{Q16_FULL} * this->half_cycle_time_us) / ramp_us;
    if (step == 0)
      step = 1;
    if (step > Q16_FULL)
      step = Q16_FULL;

    if (target < requested) {
      const uint32_t delta = requested - target;
      target += delta < step ? delta : step;
    } else {
      const uint32_t delta = target - requested;
      target -= delta < step ? delta : step;
    }
  }

  this->target_q16.store(target, std::memory_order_relaxed);
  return target;
}

void IRAM_ATTR HOT ACCycleSkipDataStore::gpio_intr() {
  const uint32_t now = micros();
  const uint32_t elapsed = now - this->crossed_zero_at;

  if (this->crossed_zero_at == 0) {
    this->reset_sync_(now);
    return;
  }

  if (elapsed < this->noise_filter_us) {
    this->rejected_crossings++;
    return;
  }

  if (elapsed < this->min_zero_cross_interval_us || elapsed > this->max_zero_cross_interval_us) {
    this->invalid_crossings++;
    this->resync_events++;
    this->reset_sync_(now);
    return;
  }

  this->half_cycle_time_us = elapsed;
  this->crossed_zero_at = now;

  if (!this->synchronized) {
    this->force_off_();
    this->valid_sync_intervals++;
    if (this->valid_sync_intervals < 2)
      return;
    this->synchronized = true;
    this->second_half_cycle = false;
    this->accumulator_q16 = 0;
    return;
  }

  const uint32_t target = this->update_target_(now);
  if (target == 0) {
    this->force_off_();
    this->accumulator_q16 = 0;
    this->second_half_cycle = false;
    return;
  }

  if (target >= Q16_FULL) {
    this->schedule_gate_pulse_();
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

  if (this->cycle_on) {
    this->schedule_gate_pulse_();
  } else {
    this->force_off_();
  }
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

bool IRAM_ATTR HOT ACCycleSkipDataStore::s_gate_timer_alarm(gptimer_handle_t timer,
                                                            const gptimer_alarm_event_data_t *edata,
                                                            void *user_ctx) {
  auto *store = static_cast<ACCycleSkipDataStore *>(user_ctx);
  if (store == nullptr)
    return false;

  if (store->gate_timer_phase.load(std::memory_order_relaxed) == ACCycleSkipGateTimerPhase::WAITING_GATE_ON) {
    const uint32_t scheduled_generation = store->scheduled_pulse_generation.load(std::memory_order_relaxed);
    const uint32_t current_generation = store->pulse_generation.load(std::memory_order_relaxed);
    if (scheduled_generation == 0 || scheduled_generation != current_generation ||
        store->requested_q16.load(std::memory_order_relaxed) == 0) {
      store->force_off_();
      return false;
    }

    store->gate_pin.digital_write(true);
    store->gate_timer_phase.store(ACCycleSkipGateTimerPhase::WAITING_GATE_OFF, std::memory_order_relaxed);

    gptimer_alarm_config_t alarm_config = {};
    alarm_config.alarm_count = edata->count_value + store->gate_pulse_us.load(std::memory_order_relaxed);
    alarm_config.flags.auto_reload_on_alarm = false;
    if (gptimer_set_alarm_action(timer, &alarm_config) != ESP_OK) {
      store->force_off_();
    }
    return false;
  }

  store->gate_pin.digital_write(false);
  store->gate_timer_phase.store(ACCycleSkipGateTimerPhase::IDLE, std::memory_order_relaxed);
  store->scheduled_pulse_generation.store(0, std::memory_order_relaxed);
  gptimer_set_alarm_action(timer, nullptr);
  return false;
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

  gptimer_config_t timer_config = {};
  timer_config.clk_src = GPTIMER_CLK_SRC_DEFAULT;
  timer_config.direction = GPTIMER_COUNT_UP;
  timer_config.resolution_hz = 1000000;

  esp_err_t err = gptimer_new_timer(&timer_config, &this->store_.gate_timer);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to create gate pulse GPTimer: %s", esp_err_to_name(err));
  } else {
    gptimer_event_callbacks_t cbs = {};
    cbs.on_alarm = &ACCycleSkipDataStore::s_gate_timer_alarm;
    err = gptimer_register_event_callbacks(this->store_.gate_timer, &cbs, &this->store_);
    if (err != ESP_OK) {
      ESP_LOGE(TAG, "Failed to register gate pulse GPTimer callback: %s", esp_err_to_name(err));
      gptimer_del_timer(this->store_.gate_timer);
      this->store_.gate_timer = nullptr;
    }
  }

  if (this->store_.gate_timer != nullptr) {
    err = gptimer_enable(this->store_.gate_timer);
    if (err != ESP_OK) {
      ESP_LOGE(TAG, "Failed to enable gate pulse GPTimer: %s", esp_err_to_name(err));
      gptimer_del_timer(this->store_.gate_timer);
      this->store_.gate_timer = nullptr;
    }
  }

  if (this->store_.gate_timer != nullptr) {
    err = gptimer_start(this->store_.gate_timer);
    if (err != ESP_OK) {
      ESP_LOGE(TAG, "Failed to start gate pulse GPTimer: %s", esp_err_to_name(err));
      gptimer_disable(this->store_.gate_timer);
      gptimer_del_timer(this->store_.gate_timer);
      this->store_.gate_timer = nullptr;
    }
  }

  if (this->store_.gate_timer == nullptr) {
    this->mark_failed();
    return;
  }

  if (setup_zero_cross_pin) {
    this->zero_cross_pin_->setup();
    this->store_.zero_cross_pin = this->zero_cross_pin_->to_isr();
    this->zero_cross_pin_->attach_interrupt(&ACCycleSkipDataStore::s_gpio_intr, &this->store_,
                                            this->zero_cross_interrupt_type_);
  }
}

void ACCycleSkipOutput::on_shutdown() {
  this->store_.force_off_();
  if (this->gate_pin_ != nullptr)
    this->gate_pin_->digital_write(false);
  this->store_.requested_q16.store(0, std::memory_order_relaxed);
  this->store_.target_q16.store(0, std::memory_order_relaxed);
  this->store_.boost_until_us.store(0, std::memory_order_relaxed);
}

void ACCycleSkipOutput::write_state(float state) {
  if (state <= 0.0f || std::isnan(state)) {
    this->store_.requested_q16.store(0, std::memory_order_relaxed);
    this->store_.target_q16.store(0, std::memory_order_relaxed);
    this->store_.boost_until_us.store(0, std::memory_order_relaxed);
    this->store_.force_off_();
    return;
  }

  uint32_t requested = Q16_FULL;
  if (state >= 1.0f) {
    requested = Q16_FULL;
  } else {
    requested = static_cast<uint32_t>(state * Q16_FULL + 0.5f);
  }

  const bool starting = this->store_.requested_q16.load(std::memory_order_relaxed) == 0 && requested > 0 &&
                        requested < Q16_FULL;
  this->store_.requested_q16.store(requested, std::memory_order_relaxed);
  const uint32_t start_boost_ms = this->store_.start_boost_ms.load(std::memory_order_relaxed);
  if (starting && start_boost_ms > 0) {
    this->store_.target_q16.store(Q16_FULL, std::memory_order_relaxed);
    this->store_.boost_until_us.store(micros() + start_boost_ms * 1000UL, std::memory_order_relaxed);
  }
}

void ACCycleSkipOutput::dump_config() {
  ESP_LOGCONFIG(TAG, "AC Cycle Skip Output:");
  LOG_PIN("  Gate Pin: ", this->gate_pin_);
  LOG_PIN("  Zero-Cross Pin: ", this->zero_cross_pin_);
  ESP_LOGCONFIG(TAG,
                "  Noise filter: %uus\n"
                "  Valid zero-cross interval: %u-%uus\n"
                "  Gate delay: %uus\n"
                "  Gate pulse: %uus\n"
                "  Start boost: %ums\n"
                "  Ramp time: %ums\n"
                "  Estimated mains: %.2f Hz\n"
                "  Rejected zero-cross pulses: %u\n"
                "  Invalid zero-cross intervals: %u\n"
                "  Resync events: %u",
                this->store_.noise_filter_us, this->store_.min_zero_cross_interval_us,
                this->store_.max_zero_cross_interval_us, this->store_.gate_delay_us.load(std::memory_order_relaxed),
                this->store_.gate_pulse_us.load(std::memory_order_relaxed),
                this->store_.start_boost_ms.load(std::memory_order_relaxed),
                this->store_.ramp_ms.load(std::memory_order_relaxed),
                1e6f / this->store_.half_cycle_time_us / 2.0f, this->store_.rejected_crossings,
                this->store_.invalid_crossings, this->store_.resync_events);
  LOG_FLOAT_OUTPUT(this);
}

}  // namespace esphome::ac_cycle_skip
