#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/core/component.h"

namespace esphome::xdb401 {

class XDB401Component final : public PollingComponent, public i2c::I2CDevice {
 public:
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { this->temperature_sensor_ = temperature_sensor; }
  void set_pressure_sensor(sensor::Sensor *pressure_sensor) { this->pressure_sensor_ = pressure_sensor; }
  void set_pressure_range_bar(uint8_t pressure_range_bar) { this->pressure_range_bar_ = pressure_range_bar; }
  void set_start_error_sensor(sensor::Sensor *sensor) { this->start_error_sensor_ = sensor; }
  void set_status_error_sensor(sensor::Sensor *sensor) { this->status_error_sensor_ = sensor; }
  void set_measurement_timeout_sensor(sensor::Sensor *sensor) { this->measurement_timeout_sensor_ = sensor; }
  void set_packet_error_sensor(sensor::Sensor *sensor) { this->packet_error_sensor_ = sensor; }
  void set_rejected_packet_sensor(sensor::Sensor *sensor) { this->rejected_packet_sensor_ = sensor; }
  void set_total_error_sensor(sensor::Sensor *sensor) { this->total_error_sensor_ = sensor; }
  void set_consecutive_error_sensor(sensor::Sensor *sensor) { this->consecutive_error_sensor_ = sensor; }
  void set_last_error_sensor(text_sensor::TextSensor *sensor) { this->last_error_sensor_ = sensor; }

  void setup() override;
  void dump_config() override;
  void update() override;

 protected:
  enum class ErrorType : uint8_t { START, STATUS, TIMEOUT, PACKET, REJECTED_PACKET };

  void handle_comm_failure_(ErrorType type, const char *message);
  void publish_diagnostics_();
  i2c::ErrorCode start_measurement_();
  void check_measurement_ready_(uint8_t attempt);
  void read_measurement_();

  static constexpr float full_scale_pressure_pa(uint8_t pressure_range_bar) { return pressure_range_bar * 100000.0f; }

  uint8_t comm_err_counter_{0};
  bool measurement_in_progress_{false};
  bool have_last_temperature_{false};
  float last_temperature_{0.0f};
  uint8_t pressure_range_bar_{10};

  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *pressure_sensor_{nullptr};
  sensor::Sensor *start_error_sensor_{nullptr};
  sensor::Sensor *status_error_sensor_{nullptr};
  sensor::Sensor *measurement_timeout_sensor_{nullptr};
  sensor::Sensor *packet_error_sensor_{nullptr};
  sensor::Sensor *rejected_packet_sensor_{nullptr};
  sensor::Sensor *total_error_sensor_{nullptr};
  sensor::Sensor *consecutive_error_sensor_{nullptr};
  text_sensor::TextSensor *last_error_sensor_{nullptr};

  uint32_t start_error_count_{0};
  uint32_t status_error_count_{0};
  uint32_t measurement_timeout_count_{0};
  uint32_t packet_error_count_{0};
  uint32_t rejected_packet_count_{0};
  uint32_t total_error_count_{0};
  uint32_t consecutive_error_count_{0};
};

}  // namespace esphome::xdb401
