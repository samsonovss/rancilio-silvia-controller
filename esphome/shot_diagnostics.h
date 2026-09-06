#pragma once

#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>

#include "esp_heap_caps.h"
#include "esphome/components/web_server_base/web_server_base.h"

namespace silvia_diag {

// Long-lived shot samples belong in external PSRAM. Critical control state,
// interrupts and the pressure loop remain in fast internal SRAM. If PSRAM is
// unexpectedly unavailable, fall back to the normal heap so brewing and CSV
// diagnostics still work instead of crashing during allocation.
template<typename T> class PsramAllocator {
 public:
  using value_type = T;

  PsramAllocator() noexcept = default;
  template<typename U> PsramAllocator(const PsramAllocator<U> &) noexcept {}

  T *allocate(std::size_t count) {
    if (count > std::numeric_limits<std::size_t>::max() / sizeof(T))
      std::abort();
    const std::size_t bytes = count * sizeof(T);
    void *memory = heap_caps_malloc(bytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (memory == nullptr)
      memory = heap_caps_malloc(bytes, MALLOC_CAP_8BIT);
    if (memory == nullptr)
      std::abort();
    return static_cast<T *>(memory);
  }

  void deallocate(T *pointer, std::size_t) noexcept { heap_caps_free(pointer); }

  template<typename U> struct rebind { using other = PsramAllocator<U>; };
};

template<typename T, typename U>
bool operator==(const PsramAllocator<T> &, const PsramAllocator<U> &) { return true; }
template<typename T, typename U>
bool operator!=(const PsramAllocator<T> &, const PsramAllocator<U> &) { return false; }

enum class StartupState : uint8_t {
  WAIT_DROP,
  WAIT_RISE,
  PI,
  SENSOR_FAIL,
  POWER,
  PUMP_OFF,
};

enum class TransitionReason : uint8_t {
  NONE,
  PRESSURE_RESPONSE,
  TIMEOUT_2S,
  START_PRESSURE_LOW,
  RESIDUAL_PRESSURE_CLEARED,
  RESIDUAL_PRESSURE_STALLED,
  RESIDUAL_PRESSURE_TIMEOUT,
  START_SENSOR_INVALID,
};

struct ShotSample {
  uint32_t elapsed_ms;
  uint8_t phase;
  float target_bar;
  float pressure_bar;
  float pressure_slope_bar_s;
  float predicted_pressure_bar;
  float time_to_target_s;
  float soft_start_limit_percent;
  float desired_pressure_slope_bar_s;
  float rise_rate_brake;
  float pressure_recovery_boost;
  float error_bar;
  bool sensor_ok;
  uint32_t sensor_age_ms;
  bool brew_valve;
  StartupState startup_state;
  TransitionReason transition_reason;
  int16_t drop_samples;
  int16_t rise_samples;
  bool pi_enabled;
  float feed_forward;
  float p_term;
  float i_term;
  float pi_output;
  float final_output_percent;
  float temperature_feed_forward_percent;
  float pressure_sensor_temperature_c;
  float boiler_temperature_c;
  float weight_g;
  float flow_g_s;
  uint32_t xdb_start_errors;
  uint32_t xdb_status_errors;
  uint32_t xdb_measurement_timeouts;
  uint32_t xdb_packet_errors;
  uint32_t xdb_rejected_packets;
  uint32_t xdb_total_errors;
  uint32_t xdb_consecutive_errors;
  uint32_t xdb_link_errors;
  uint32_t xdb_pressure_rejected;
  uint32_t xdb_temperature_rejected;
  uint8_t phase_index;
  uint8_t phase_kind;
  bool phase_pump_enabled;
  float phase_progress;
  float phase_elapsed_s;
  float phase_remaining_s;
  float phase_start_target_bar;
  float phase_end_target_bar;
  float target_plus_025s_bar;
  float target_plus_05s_bar;
  float target_plus_1s_bar;
  float target_slope_bar_s;
  float predicted_target_bar;
  float previous_output_percent;
  bool scale_data_valid;
  uint8_t profile_code;
  float brew_target_temperature_c;
  float coffee_dose_g;
  float target_weight_g;
  bool stop_by_weight_enabled;
  float startup_initial_pressure_bar;
  uint32_t startup_wait_ms;
};

// Hard ceiling on a single captured shot. Raised together with the move of the
// capture loop from 200 ms to 100 ms so the covered duration stayed the same.
inline constexpr size_t MAX_SAMPLES = 1200;

inline std::vector<ShotSample, PsramAllocator<ShotSample>> last_shot;
inline bool capture_active = false;
inline bool csv_ready = false;
inline uint32_t shot_started_ms = 0;
inline TransitionReason last_transition_reason = TransitionReason::NONE;

inline const char *startup_state_name(StartupState state) {
  switch (state) {
    case StartupState::WAIT_DROP:
      return "WAIT_DROP";
    case StartupState::WAIT_RISE:
      return "WAIT_RISE";
    case StartupState::PI:
      return "PI";
    case StartupState::SENSOR_FAIL:
      return "SENSOR_FAIL";
    case StartupState::POWER:
      return "POWER";
    case StartupState::PUMP_OFF:
      return "PUMP_OFF";
  }
  return "UNKNOWN";
}

inline const char *transition_reason_name(TransitionReason reason) {
  switch (reason) {
    case TransitionReason::PRESSURE_RESPONSE:
      return "pressure_response";
    case TransitionReason::TIMEOUT_2S:
      return "timeout_2s";
    case TransitionReason::START_PRESSURE_LOW:
      return "start_pressure_low";
    case TransitionReason::RESIDUAL_PRESSURE_CLEARED:
      return "residual_pressure_cleared";
    case TransitionReason::RESIDUAL_PRESSURE_STALLED:
      return "residual_pressure_stalled";
    case TransitionReason::RESIDUAL_PRESSURE_TIMEOUT:
      return "residual_pressure_timeout";
    case TransitionReason::START_SENSOR_INVALID:
      return "start_sensor_invalid";
    case TransitionReason::NONE:
      return "";
  }
  return "";
}

inline void begin_shot(bool enabled, uint32_t now) {
  capture_active = enabled;
  if (!enabled)
    return;
  last_shot.clear();
  // The capture loop runs at 100 ms, so 1200 samples cover two minutes of
  // brewing. The buffer lives in PSRAM (~250 B per sample, ~300 kB total),
  // which is nothing against the 8 MB available.
  last_shot.reserve(MAX_SAMPLES);
  csv_ready = false;
  shot_started_ms = now;
  last_transition_reason = TransitionReason::NONE;
}

inline void record(const ShotSample &sample) {
  if (!capture_active || last_shot.size() >= MAX_SAMPLES)
    return;
  last_shot.push_back(sample);
}

inline void finish_shot() {
  if (!capture_active)
    return;
  capture_active = false;
  csv_ready = !last_shot.empty();
}

inline void clear() {
  capture_active = false;
  csv_ready = false;
  last_shot.clear();
  last_transition_reason = TransitionReason::NONE;
}

using PsramString = std::basic_string<char, std::char_traits<char>, PsramAllocator<char>>;

inline PsramString make_csv() {
  PsramString csv;
  csv.reserve(768 + last_shot.size() * 420);
  csv += "elapsed_ms,phase,target_bar,pressure_bar,pressure_slope_bar_s,predicted_pressure_bar,";
  csv += "time_to_target_s,soft_start_limit_percent,";
  csv += "desired_pressure_slope_bar_s,rise_rate_brake,pressure_recovery_boost,";
  csv += "error_bar,sensor_ok,sensor_age_ms,brew_valve,";
  csv += "startup_state,transition_reason,drop_samples,rise_samples,pi_enabled,feed_forward,";
  csv += "p_term,i_term,pi_output,final_output_percent,";
  csv += "temperature_feed_forward_percent,pressure_sensor_temperature_c,boiler_temperature_c,";
  csv += "weight_g,flow_g_s,xdb_start_errors,xdb_status_errors,xdb_measurement_timeouts,";
  csv += "xdb_packet_errors,xdb_rejected_packets,xdb_total_errors,xdb_consecutive_errors,";
  csv += "xdb_link_errors,xdb_pressure_rejected,xdb_temperature_rejected,";
  csv += "phase_index,phase_kind,phase_pump_enabled,phase_progress,phase_elapsed_s,phase_remaining_s,";
  csv += "phase_start_target_bar,phase_end_target_bar,target_plus_025s_bar,target_plus_05s_bar,";
  csv += "target_plus_1s_bar,target_slope_bar_s,predicted_target_bar,previous_output_percent,scale_data_valid,";
  csv += "profile_code,brew_target_temperature_c,coffee_dose_g,target_weight_g,stop_by_weight_enabled,";
  csv += "startup_initial_pressure_bar,startup_wait_ms\n";
  char line[768];
  for (const auto &sample : last_shot) {
    const int length = snprintf(
        line, sizeof(line),
        "%lu,%u,%.4f,%.4f,%.4f,%.4f,%.4f,%.2f,%.4f,%.5f,%.5f,%.4f,%u,%lu,%u,%s,%s,%d,%d,%u,"
        "%.5f,%.5f,%.5f,%.5f,%.2f,%.2f,%.2f,%.2f,%.3f,%.3f,%lu,%lu,%lu,%lu,%lu,%lu,%lu,"
        "%lu,%lu,%lu,"
        "%u,%u,%u,%.5f,%.3f,%.3f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.2f,%u,"
        "%u,%.2f,%.2f,%.2f,%u,%.4f,%lu\n",
        static_cast<unsigned long>(sample.elapsed_ms), sample.phase,
        sample.target_bar, sample.pressure_bar, sample.pressure_slope_bar_s,
        sample.predicted_pressure_bar, sample.time_to_target_s,
        sample.soft_start_limit_percent,
        sample.desired_pressure_slope_bar_s, sample.rise_rate_brake,
        sample.pressure_recovery_boost,
        sample.error_bar,
        sample.sensor_ok ? 1 : 0,
        static_cast<unsigned long>(sample.sensor_age_ms),
        sample.brew_valve ? 1 : 0, startup_state_name(sample.startup_state),
        transition_reason_name(sample.transition_reason), sample.drop_samples,
        sample.rise_samples, sample.pi_enabled ? 1 : 0, sample.feed_forward,
        sample.p_term, sample.i_term, sample.pi_output,
        sample.final_output_percent, sample.temperature_feed_forward_percent,
        sample.pressure_sensor_temperature_c, sample.boiler_temperature_c,
        sample.weight_g, sample.flow_g_s,
        static_cast<unsigned long>(sample.xdb_start_errors),
        static_cast<unsigned long>(sample.xdb_status_errors),
        static_cast<unsigned long>(sample.xdb_measurement_timeouts),
        static_cast<unsigned long>(sample.xdb_packet_errors),
        static_cast<unsigned long>(sample.xdb_rejected_packets),
        static_cast<unsigned long>(sample.xdb_total_errors),
        static_cast<unsigned long>(sample.xdb_consecutive_errors),
        static_cast<unsigned long>(sample.xdb_link_errors),
        static_cast<unsigned long>(sample.xdb_pressure_rejected),
        static_cast<unsigned long>(sample.xdb_temperature_rejected),
        sample.phase_index, sample.phase_kind,
        sample.phase_pump_enabled ? 1 : 0, sample.phase_progress,
        sample.phase_elapsed_s, sample.phase_remaining_s,
        sample.phase_start_target_bar, sample.phase_end_target_bar,
        sample.target_plus_025s_bar, sample.target_plus_05s_bar,
        sample.target_plus_1s_bar, sample.target_slope_bar_s,
        sample.predicted_target_bar, sample.previous_output_percent,
        sample.scale_data_valid ? 1 : 0, sample.profile_code,
        sample.brew_target_temperature_c, sample.coffee_dose_g,
        sample.target_weight_g, sample.stop_by_weight_enabled ? 1 : 0,
        sample.startup_initial_pressure_bar,
        static_cast<unsigned long>(sample.startup_wait_ms));
    if (length > 0)
      csv.append(line, std::min<int>(length, sizeof(line) - 1));
  }
  return csv;
}

class ShotCsvHandler : public AsyncWebHandler {
 public:
  bool canHandle(AsyncWebServerRequest *request) const override {
    if (request->method() != HTTP_GET)
      return false;
    char buffer[AsyncWebServerRequest::URL_BUF_SIZE];
    return request->url_to(buffer) == "/shot.csv";
  }

  void handleRequest(AsyncWebServerRequest *request) override {
    if (!csv_ready) {
      request->send(404, "text/plain; charset=utf-8", "Diagnostic CSV is not ready");
      return;
    }
    // web_server_idf copies a std::string response into another std::string.
    // A full diagnostic CSV can therefore require two large internal-heap
    // buffers and abort the ESP32 when the second allocation fails. Keep the
    // only CSV buffer in PSRAM and use the pointer/length response overload,
    // which httpd sends synchronously before csv goes out of scope.
    const PsramString csv = make_csv();
    auto *response = request->beginResponse(
        200, "text/csv; charset=utf-8",
        reinterpret_cast<const uint8_t *>(csv.data()), csv.size());
    response->addHeader("Content-Disposition", "attachment; filename=\"rancilio-last-shot.csv\"");
    response->addHeader("Cache-Control", "no-store");
    request->send(response);
  }
};

inline ShotCsvHandler shot_csv_handler;
inline bool handler_registered = false;

inline void register_web_handler() {
  if (handler_registered || esphome::web_server_base::global_web_server_base == nullptr)
    return;
  esphome::web_server_base::global_web_server_base->add_handler(&shot_csv_handler);
  handler_registered = true;
}

}  // namespace silvia_diag
