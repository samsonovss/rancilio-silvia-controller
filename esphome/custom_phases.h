#pragma once

#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>

#include "esphome/components/web_server_base/web_server_base.h"
#include "esphome/core/log.h"
#include "shot_profiles.h"
#include "user_profiles.h"

namespace silvia_custom_phases {

static constexpr const char *TAG = "custom_phases";

inline bool parse_float(const std::string &value, float minimum, float maximum,
                        float &result) {
  char *tail = nullptr;
  result = strtof(value.c_str(), &tail);
  return tail != value.c_str() && tail != nullptr && *tail == '\0' &&
         std::isfinite(result) && result >= minimum && result <= maximum;
}

inline std::vector<std::string> split(const std::string &value, char delimiter) {
  std::vector<std::string> parts;
  size_t start = 0;
  while (start <= value.size()) {
    const size_t end = value.find(delimiter, start);
    parts.push_back(value.substr(start, end == std::string::npos
                                           ? std::string::npos
                                           : end - start));
    if (end == std::string::npos)
      break;
    start = end + 1;
  }
  return parts;
}

inline bool parse_spec(const std::string &spec,
                       std::vector<silvia::ShotPhase> &phases) {
  std::string error;
  return silvia_profiles::parse_phase_spec(spec, phases, &error) &&
         silvia::set_custom_shot_phases(phases, &error);
}

inline void send_json(AsyncWebServerRequest *request, int code,
                      const std::string &json) {
  auto *response = request->beginResponse(
      code, "application/json; charset=utf-8", json);
  response->addHeader("Cache-Control", "no-store");
  response->addHeader("X-Content-Type-Options", "nosniff");
  request->send(response);
}

class CustomPhasesHandler : public AsyncWebHandler {
 public:
  bool canHandle(AsyncWebServerRequest *request) const override {
    char buffer[AsyncWebServerRequest::URL_BUF_SIZE];
    const std::string url = request->url_to(buffer);
    return request->method() == HTTP_POST &&
           (url == "/custom-phases/apply" || url == "/custom-phases/clear");
  }

  void handleRequest(AsyncWebServerRequest *request) override {
    char buffer[AsyncWebServerRequest::URL_BUF_SIZE];
    const std::string url = request->url_to(buffer);
    if (url == "/custom-phases/clear") {
      silvia::clear_custom_shot_phases();
      send_json(request, 200, "{\"ok\":true}");
      return;
    }
    if (!request->hasParam("spec")) {
      send_json(request, 400, "{\"ok\":false,\"error\":\"missing_spec\"}");
      return;
    }
    std::vector<silvia::ShotPhase> phases;
    if (!parse_spec(request->getParam("spec")->value(), phases)) {
      send_json(request, 400, "{\"ok\":false,\"error\":\"invalid_spec\"}");
      return;
    }
    ESP_LOGI(TAG, "Applied %u custom phases", static_cast<unsigned>(phases.size()));
    send_json(request, 200, "{\"ok\":true}");
  }
};

inline CustomPhasesHandler custom_phases_handler;
inline bool handler_registered = false;

inline void register_web_handler() {
  if (handler_registered ||
      esphome::web_server_base::global_web_server_base == nullptr)
    return;
  esphome::web_server_base::global_web_server_base->add_handler(
      &custom_phases_handler);
  handler_registered = true;
}

}  // namespace silvia_custom_phases
