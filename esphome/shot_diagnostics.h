#pragma once

#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <string>
#include <vector>

#include "esphome/components/web_server_base/web_server_base.h"

namespace silvia_diag {

enum class StartupState : uint8_t {
  WAIT_DROP,
  WAIT_RISE,
  HANDOFF,
  PI,
  SENSOR_FAIL,
  POWER,
  PUMP_OFF,
};

enum class TransitionReason : uint8_t {
  NONE,
  PRESSURE_RETURN,
  TIMEOUT_2S,
};

struct ShotSample {
  uint32_t elapsed_ms;
  uint8_t phase;
  float target_bar;
  float pressure_bar;
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
  float handoff_percent;
  float final_output_percent;
};

inline std::vector<ShotSample> last_shot;
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
    case StartupState::HANDOFF:
      return "HANDOFF";
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
    case TransitionReason::PRESSURE_RETURN:
      return "pressure_return";
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

inline std::string make_csv() {
  std::string csv;
  csv.reserve(256 + last_shot.size() * 180);
  csv += "elapsed_ms,phase,target_bar,pressure_bar,error_bar,sensor_ok,sensor_age_ms,brew_valve,";
  csv += "startup_state,transition_reason,drop_samples,rise_samples,pi_enabled,feed_forward,";
  csv += "p_term,i_term,pi_output,handoff_percent,final_output_percent\n";
  char line[384];
  for (const auto &sample : last_shot) {
    const int length = snprintf(
        line, sizeof(line),
        "%lu,%u,%.4f,%.4f,%.4f,%u,%lu,%u,%s,%s,%d,%d,%u,%.5f,%.5f,%.5f,%.5f,%.2f,%.2f\n",
        static_cast<unsigned long>(sample.elapsed_ms), sample.phase,
        sample.target_bar, sample.pressure_bar, sample.error_bar,
        sample.sensor_ok ? 1 : 0,
        static_cast<unsigned long>(sample.sensor_age_ms),
        sample.brew_valve ? 1 : 0, startup_state_name(sample.startup_state),
        transition_reason_name(sample.transition_reason), sample.drop_samples,
        sample.rise_samples, sample.pi_enabled ? 1 : 0, sample.feed_forward,
        sample.p_term, sample.i_term, sample.pi_output, sample.handoff_percent,
        sample.final_output_percent);
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
    auto *response = request->beginResponse(200, "text/csv; charset=utf-8", make_csv());
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
