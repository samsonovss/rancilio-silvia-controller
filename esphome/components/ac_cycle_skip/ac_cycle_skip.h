#pragma once

#include "esphome/components/output/float_output.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include <driver/gptimer.h>

namespace esphome::ac_cycle_skip {

enum class ACCycleSkipGateTimerPhase : uint8_t {
  IDLE = 0,
  WAITING_GATE_ON,
  WAITING_GATE_OFF,
};

struct ACCycleSkipDataStore {
  ISRInternalGPIOPin gate_pin;
  ISRInternalGPIOPin zero_cross_pin;
  uint8_t zero_cross_pin_number;
  gptimer_handle_t gate_timer{nullptr};

  volatile uint32_t target_q16{0};
  volatile uint32_t requested_q16{0};
  volatile uint32_t boost_until_us{0};
  uint32_t accumulator_q16{0};
  uint32_t crossed_zero_at{0};
  uint32_t half_cycle_time_us{10000};
  uint32_t noise_filter_us{5000};
  uint32_t min_zero_cross_interval_us{6000};
  uint32_t max_zero_cross_interval_us{13000};
  uint32_t gate_delay_us{100};
  uint32_t gate_pulse_us{300};
  uint32_t start_boost_ms{0};
  uint32_t ramp_ms{800};
  uint32_t rejected_crossings{0};
  uint32_t invalid_crossings{0};
  uint32_t resync_events{0};
  uint8_t valid_sync_intervals{0};
  bool synchronized{false};
  bool second_half_cycle{false};
  bool cycle_on{false};
  volatile ACCycleSkipGateTimerPhase gate_timer_phase{ACCycleSkipGateTimerPhase::IDLE};

  void gpio_intr();
  void force_off_();
  void schedule_gate_pulse_();
  void reset_sync_(uint32_t now);
  uint32_t update_target_(uint32_t now);
  static void s_gpio_intr(ACCycleSkipDataStore *store);
  static bool s_gate_timer_alarm(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);
};

class ACCycleSkipOutput final : public output::FloatOutput, public Component {
 public:
  void setup() override;
  void dump_config() override;
  void on_shutdown() override;

  void set_gate_pin(InternalGPIOPin *gate_pin) { gate_pin_ = gate_pin; }
  void set_zero_cross_pin(InternalGPIOPin *zero_cross_pin) { zero_cross_pin_ = zero_cross_pin; }
  void set_zero_cross_interrupt_type(gpio::InterruptType type) { zero_cross_interrupt_type_ = type; }
  void set_noise_filter_us(uint32_t noise_filter_us) { store_.noise_filter_us = noise_filter_us; }
  void set_min_zero_cross_interval_us(uint32_t min_us) { store_.min_zero_cross_interval_us = min_us; }
  void set_max_zero_cross_interval_us(uint32_t max_us) { store_.max_zero_cross_interval_us = max_us; }
  void set_gate_delay_us(uint32_t gate_delay_us) { store_.gate_delay_us = gate_delay_us; }
  void set_gate_pulse_us(uint32_t gate_pulse_us) { store_.gate_pulse_us = gate_pulse_us; }
  void set_start_boost_ms(uint32_t start_boost_ms) { store_.start_boost_ms = start_boost_ms; }
  void set_ramp_ms(uint32_t ramp_ms) { store_.ramp_ms = ramp_ms; }

 protected:
  void write_state(float state) override;

  InternalGPIOPin *gate_pin_{nullptr};
  InternalGPIOPin *zero_cross_pin_{nullptr};
  gpio::InterruptType zero_cross_interrupt_type_{gpio::INTERRUPT_FALLING_EDGE};
  ACCycleSkipDataStore store_;
};

}  // namespace esphome::ac_cycle_skip
