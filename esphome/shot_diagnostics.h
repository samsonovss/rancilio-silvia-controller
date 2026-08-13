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
};

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
  last_shot.reserve(600);
  csv_ready = false;
  shot_started_ms = now;
  last_transition_reason = TransitionReason::NONE;
}

inline void record(const ShotSample &sample) {
  if (!capture_active || last_shot.size() >= 600)
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
  csv.reserve(512 + last_shot.size() * 260);
  csv += "elapsed_ms,phase,target_bar,pressure_bar,pressure_slope_bar_s,predicted_pressure_bar,";
  csv += "time_to_target_s,soft_start_limit_percent,";
  csv += "desired_pressure_slope_bar_s,rise_rate_brake,pressure_recovery_boost,";
  csv += "error_bar,sensor_ok,sensor_age_ms,brew_valve,";
  csv += "startup_state,transition_reason,drop_samples,rise_samples,pi_enabled,feed_forward,";
  csv += "p_term,i_term,pi_output,final_output_percent,";
  csv += "temperature_feed_forward_percent,pressure_sensor_temperature_c,boiler_temperature_c,";
  csv += "weight_g,flow_g_s,xdb_start_errors,xdb_status_errors,xdb_measurement_timeouts,";
  csv += "xdb_packet_errors,xdb_rejected_packets,xdb_total_errors,xdb_consecutive_errors\n";
  char line[512];
  for (const auto &sample : last_shot) {
    const int length = snprintf(
        line, sizeof(line),
        "%lu,%u,%.4f,%.4f,%.4f,%.4f,%.4f,%.2f,%.4f,%.5f,%.5f,%.4f,%u,%lu,%u,%s,%s,%d,%d,%u,"
        "%.5f,%.5f,%.5f,%.5f,%.2f,%.2f,%.2f,%.2f,%.3f,%.3f,%lu,%lu,%lu,%lu,%lu,%lu,%lu\n",
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
        static_cast<unsigned long>(sample.xdb_consecutive_errors));
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
