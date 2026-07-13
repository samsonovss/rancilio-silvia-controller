#pragma once

#include "esphome/components/output/float_output.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"

namespace esphome::ac_cycle_skip {

struct ACCycleSkipDataStore {
  ISRInternalGPIOPin gate_pin;
  ISRInternalGPIOPin zero_cross_pin;
  uint8_t zero_cross_pin_number;

  volatile uint32_t target_q16{0};
  volatile uint32_t requested_q16{0};
  volatile uint32_t boost_until_us{0};
  uint32_t accumulator_q16{0};
  uint32_t crossed_zero_at{0};
  uint32_t half_cycle_time_us{10000};
  uint32_t noise_filter_us{5000};
  uint32_t start_boost_ms{0};
  uint32_t ramp_ms{800};
  uint32_t rejected_crossings{0};
  bool second_half_cycle{false};
  bool cycle_on{false};

  void gpio_intr();
  uint32_t update_target_(uint32_t now);
  static void s_gpio_intr(ACCycleSkipDataStore *store);
};

class ACCycleSkipOutput final : public output::FloatOutput, public Component {
 public:
  void setup() override;
  void dump_config() override;

  void set_gate_pin(InternalGPIOPin *gate_pin) { gate_pin_ = gate_pin; }
  void set_zero_cross_pin(InternalGPIOPin *zero_cross_pin) { zero_cross_pin_ = zero_cross_pin; }
  void set_zero_cross_interrupt_type(gpio::InterruptType type) { zero_cross_interrupt_type_ = type; }
  void set_noise_filter_us(uint32_t noise_filter_us) { store_.noise_filter_us = noise_filter_us; }
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
