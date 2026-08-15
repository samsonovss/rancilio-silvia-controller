#pragma once

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <string>
#include <vector>

#include "esphome/core/log.h"
#include "esphome/components/web_server_base/web_server_base.h"

#include "shot_archive.h"
#include "shot_profiles.h"

namespace silvia_profiles {

static constexpr const char *TAG = "user_profiles";
static constexpr const char *FILE_PREFIX = "profile-";
static constexpr const char *FILE_SUFFIX = ".json";

static constexpr size_t MAX_PROFILES = 30;
static constexpr size_t MAX_PHASE_SPEC_LENGTH = 2048;

// -----------------------------------------------------------------------------
// Stored profile
// -----------------------------------------------------------------------------
//
// The old numeric fields are intentionally kept. This makes already existing
// profiles and the current dashboard fully backward compatible.
//
// "phases" is the new universal phase description used by the visual editor.
// Format:
//   P,<seconds>
//   I,<seconds>,<start_bar>,<end_bar>,<stop_weight_fraction>  preinfusion
//   B,<seconds>,<start_bar>,<end_bar>,<stop_weight_fraction>  delivery
//
// Multiple phases are separated with ';', for example:
//   I,12,0.5,0.5,0;P,28;B,30,0.8,0.8,0.35
//
// An empty string means "legacy profile without a custom phase list".
// -----------------------------------------------------------------------------

struct ProfileValues {
  float brew_target;
  float target_weight;
  float shot_seconds;
  float pre_pump;
  float pre_pause;
  float pre_power;
  float main_pressure;
  float end_pressure;
  float soft_infusion_time;
  float soft_infusion_start_power;

  std::string phases;
};

// -----------------------------------------------------------------------------
// Generic helpers
// -----------------------------------------------------------------------------

inline std::string json_escape(const std::string &value) {
  std::string result;
  result.reserve(value.size() + 8);

  for (const unsigned char c : value) {
    switch (c) {
      case '\"':
        result += "\\\"";
        break;
      case '\\':
        result += "\\\\";
        break;
      case '\b':
        result += "\\b";
        break;
      case '\f':
        result += "\\f";
        break;
      case '\n':
        result += "\\n";
        break;
      case '\r':
        result += "\\r";
        break;
      case '\t':
        result += "\\t";
        break;
      default:
        if (c < 0x20) {
          char escaped[7];
          snprintf(
              escaped,
              sizeof(escaped),
              "\\u%04x",
              static_cast<unsigned>(c));
          result += escaped;
        } else {
          result.push_back(static_cast<char>(c));
        }
    }
  }

  return result;
}

inline bool valid_id(const std::string &id) {
  return !id.empty() &&
         id.size() <= 64 &&
         std::all_of(
             id.begin(),
             id.end(),
             [](unsigned char c) {
               return (c >= 'a' && c <= 'z') ||
                      (c >= 'A' && c <= 'Z') ||
                      (c >= '0' && c <= '9') ||
                      c == '-' ||
                      c == '_';
             });
}

inline bool valid_name(const std::string &name) {
  if (name.empty() || name.size() > 128)
    return false;

  return std::none_of(
      name.begin(),
      name.end(),
      [](unsigned char c) {
        return c < 0x20 || c == 0x7f;
      });
}

inline bool valid_timestamp(const std::string &value) {
  return !value.empty() &&
         value.size() <= 16 &&
         std::all_of(
             value.begin(),
             value.end(),
             [](unsigned char c) {
               return c >= '0' && c <= '9';
             });
}

inline std::string profile_path(const std::string &id) {
  return std::string(silvia_archive::BASE_PATH) +
         "/" +
         FILE_PREFIX +
         id +
         FILE_SUFFIX;
}

inline bool is_profile_file(const std::string &name) {
  const size_t prefix_length = strlen(FILE_PREFIX);
  const size_t suffix_length = strlen(FILE_SUFFIX);

  return name.size() > prefix_length + suffix_length &&
         name.compare(0, prefix_length, FILE_PREFIX) == 0 &&
         name.compare(
             name.size() - suffix_length,
             suffix_length,
             FILE_SUFFIX) == 0;
}

inline std::vector<std::string> list_profile_files() {
  std::vector<std::string> names;

  if (!silvia_archive::mounted && !silvia_archive::mount())
    return names;

  DIR *directory = opendir(silvia_archive::BASE_PATH);
  if (directory == nullptr)
    return names;

  while (dirent *entry = readdir(directory)) {
    const std::string name = entry->d_name;
    if (is_profile_file(name))
      names.push_back(name);
  }

  closedir(directory);
  std::sort(names.begin(), names.end());

  return names;
}

inline bool read_file(
    const std::string &path,
    std::string &contents) {
  FILE *file = fopen(path.c_str(), "rb");
  if (file == nullptr)
    return false;

  if (fseek(file, 0, SEEK_END) != 0) {
    fclose(file);
    return false;
  }

  const long size = ftell(file);

  // A profile with 12 phases is still tiny, but leave comfortable room for
  // future metadata while rejecting unexpectedly large/corrupt files.
  if (size < 2 || size > 16384) {
    fclose(file);
    return false;
  }

  rewind(file);
  contents.resize(static_cast<size_t>(size));

  const bool ok =
      fread(contents.data(), 1, contents.size(), file) ==
      contents.size();

  fclose(file);
  return ok;
}

inline bool write_atomic(
    const std::string &path,
    const std::string &contents) {
  const std::string temporary = path + ".tmp";

  FILE *file = fopen(temporary.c_str(), "wb");
  if (file == nullptr)
    return false;

  const bool written =
      fwrite(contents.data(), 1, contents.size(), file) ==
      contents.size();

  fflush(file);
  fclose(file);

  if (!written) {
    std::remove(temporary.c_str());
    return false;
  }

  if (std::rename(temporary.c_str(), path.c_str()) == 0)
    return true;

  std::remove(path.c_str());

  if (std::rename(temporary.c_str(), path.c_str()) == 0)
    return true;

  std::remove(temporary.c_str());
  return false;
}

// -----------------------------------------------------------------------------
// Number parsing
// -----------------------------------------------------------------------------

inline bool parse_number_string(
    const std::string &value,
    float minimum,
    float maximum,
    float &result) {
  if (value.empty())
    return false;

  char *tail = nullptr;
  result = strtof(value.c_str(), &tail);

  return tail != value.c_str() &&
         tail != nullptr &&
         *tail == '\0' &&
         std::isfinite(result) &&
         result >= minimum &&
         result <= maximum;
}

inline bool parse_float(
    AsyncWebServerRequest *request,
    const char *name,
    float minimum,
    float maximum,
    float &result) {
  if (!request->hasParam(name))
    return false;

  return parse_number_string(
      request->getParam(name)->value(),
      minimum,
      maximum,
      result);
}

// -----------------------------------------------------------------------------
// Universal phase specification
// -----------------------------------------------------------------------------

inline std::vector<std::string> split(
    const std::string &text,
    char delimiter) {
  std::vector<std::string> parts;

  size_t begin = 0;
  while (begin <= text.size()) {
    const size_t end = text.find(delimiter, begin);

    if (end == std::string::npos) {
      parts.push_back(text.substr(begin));
      break;
    }

    parts.push_back(text.substr(begin, end - begin));
    begin = end + 1;
  }

  return parts;
}

inline bool fail_phase_parse(
    std::string *error,
    const char *message) {
  if (error != nullptr)
    *error = message;
  return false;
}

// Parse the compact browser format into the same ShotPhase objects used by the
// shot runner. coffee_dashboard.h can reuse this function when it receives
// /custom-phases/apply, so storage and execution use exactly the same rules.
inline bool parse_phase_spec(
    const std::string &spec,
    std::vector<silvia::ShotPhase> &phases,
    std::string *error = nullptr) {
  phases.clear();

  if (error != nullptr)
    error->clear();

  if (spec.empty())
    return true;

  if (spec.size() > MAX_PHASE_SPEC_LENGTH)
    return fail_phase_parse(error, "phase_spec_too_long");

  const auto records = split(spec, ';');

  if (records.empty())
    return fail_phase_parse(error, "empty_phase_spec");

  if (records.size() > silvia::MAX_CUSTOM_SHOT_PHASES)
    return fail_phase_parse(error, "too_many_phases");

  uint64_t total_ms = 0;

  for (const std::string &record : records) {
    if (record.empty())
      return fail_phase_parse(error, "empty_phase");

    const auto fields = split(record, ',');

    if (fields.size() == 2 && fields[0] == "P") {
      float duration_seconds = 0.0f;

      if (!parse_number_string(
              fields[1],
              0.5f,
              240.0f,
              duration_seconds)) {
        return fail_phase_parse(error, "invalid_pause_duration");
      }

      const uint32_t duration_ms =
          silvia::seconds_to_ms(duration_seconds);

      total_ms += duration_ms;
      if (total_ms > silvia::MAX_CUSTOM_SHOT_DURATION_MS)
        return fail_phase_parse(error, "profile_too_long");

      phases.push_back(
          silvia::make_pause_phase(duration_ms, "Пауза"));

      continue;
    }

    if (fields.size() == 5 &&
        (fields[0] == "B" || fields[0] == "I")) {
      const bool preinfusion = fields[0] == "I";
      float duration_seconds = 0.0f;
      float start_bar = 0.0f;
      float end_bar = 0.0f;
      float stop_weight_fraction = 0.0f;

      if (!parse_number_string(
              fields[1],
              0.5f,
              240.0f,
              duration_seconds)) {
        return fail_phase_parse(error, "invalid_brew_duration");
      }

      if (!parse_number_string(
              fields[2],
              0.0f,
              12.0f,
              start_bar) ||
          !parse_number_string(
              fields[3],
              0.0f,
              12.0f,
              end_bar)) {
        return fail_phase_parse(error, "invalid_pressure");
      }

      if (!parse_number_string(
              fields[4],
              0.0f,
              1.0f,
              stop_weight_fraction)) {
        return fail_phase_parse(error, "invalid_stop_weight");
      }

      const uint32_t duration_ms =
          silvia::seconds_to_ms(duration_seconds);

      total_ms += duration_ms;
      if (total_ms > silvia::MAX_CUSTOM_SHOT_DURATION_MS)
        return fail_phase_parse(error, "profile_too_long");

      phases.push_back(
          silvia::make_delivery_phase(
              duration_ms,
              start_bar,
              end_bar,
              stop_weight_fraction,
              preinfusion ? silvia::ShotPhaseKind::PREFUSION
                          : silvia::ShotPhaseKind::BREW,
              preinfusion ? "Предсмачивание" : "Подача"));

      continue;
    }

    return fail_phase_parse(error, "invalid_phase");
  }

  // Give the first delivery before a pause the legacy PREFUSION kind. The new
  // editor itself only cares about delivery/pause, but the existing shot status
  // and diagnostics still benefit from this internal distinction.
  for (size_t i = 0; i < phases.size(); ++i) {
    if (!phases[i].pump_enabled)
      continue;

    bool pause_after = false;
    for (size_t j = i + 1; j < phases.size(); ++j) {
      if (!phases[j].pump_enabled) {
        pause_after = true;
        break;
      }
      if (phases[j].pump_enabled)
        break;
    }

    if (i == 0 && pause_after &&
        phases[i].kind != silvia::ShotPhaseKind::PREFUSION) {
      phases[i].kind = silvia::ShotPhaseKind::PREFUSION;
      phases[i].status = "Предсмачивание";
    }

    break;
  }

  std::string validation_error;
  if (!silvia::validate_custom_shot_phases(
          phases,
          &validation_error)) {
    if (error != nullptr) {
      *error = validation_error.empty()
                   ? "invalid_phase_profile"
                   : validation_error;
    }
    phases.clear();
    return false;
  }

  return true;
}

inline bool valid_phase_spec(
    const std::string &spec,
    std::string *error = nullptr) {
  if (spec.empty()) {
    if (error != nullptr)
      error->clear();
    return true;
  }

  std::vector<silvia::ShotPhase> phases;
  return parse_phase_spec(spec, phases, error);
}

inline std::string phase_number(
    float value,
    int precision = 3) {
  char buffer[32];

  if (precision <= 0) {
    snprintf(
        buffer,
        sizeof(buffer),
        "%.0f",
        static_cast<double>(value));
  } else if (precision == 1) {
    snprintf(
        buffer,
        sizeof(buffer),
        "%.1f",
        static_cast<double>(value));
  } else {
    snprintf(
        buffer,
        sizeof(buffer),
        "%.3f",
        static_cast<double>(value));
  }

  std::string result(buffer);

  // Trim trailing zeroes from decimals to keep profile files readable.
  const size_t dot = result.find('.');
  if (dot != std::string::npos) {
    while (!result.empty() && result.back() == '0')
      result.pop_back();
    if (!result.empty() && result.back() == '.')
      result.pop_back();
  }

  return result.empty() ? "0" : result;
}

inline std::string make_phase_spec(
    const std::vector<silvia::ShotPhase> &phases) {
  std::string spec;

  for (size_t i = 0; i < phases.size(); ++i) {
    const auto &phase = phases[i];

    if (i != 0)
      spec += ';';

    if (!phase.pump_enabled) {
      spec += "P,";
      spec += phase_number(
          silvia::ms_to_seconds(phase.duration_ms),
          1);
      continue;
    }

    spec += phase.kind == silvia::ShotPhaseKind::PREFUSION ? "I," : "B,";
    spec += phase_number(
        silvia::ms_to_seconds(phase.duration_ms),
        1);
    spec += ',';
    spec += phase_number(phase.start_value, 1);
    spec += ',';
    spec += phase_number(phase.end_value, 1);
    spec += ',';
    spec += phase_number(
        phase.stop_weight_fraction,
        3);
  }

  return spec;
}

// -----------------------------------------------------------------------------
// Request / JSON conversion
// -----------------------------------------------------------------------------

inline bool read_values(
    AsyncWebServerRequest *request,
    ProfileValues &values) {
  if (!parse_float(
          request,
          "brewTarget",
          80.0f,
          105.0f,
          values.brew_target) ||
      !parse_float(
          request,
          "targetWeight",
          5.0f,
          100.0f,
          values.target_weight) ||
      !parse_float(
          request,
          "shotSeconds",
          1.0f,
          240.0f,
          values.shot_seconds) ||
      !parse_float(
          request,
          "prePump",
          0.0f,
          30.0f,
          values.pre_pump) ||
      !parse_float(
          request,
          "prePause",
          0.0f,
          120.0f,
          values.pre_pause) ||
      !parse_float(
          request,
          "prePower",
          0.0f,
          12.0f,
          values.pre_power) ||
      !parse_float(
          request,
          "mainPressure",
          0.0f,
          12.0f,
          values.main_pressure) ||
      !parse_float(
          request,
          "endPressure",
          0.0f,
          12.0f,
          values.end_pressure) ||
      !parse_float(
          request,
          "softInfusionTime",
          0.0f,
          30.0f,
          values.soft_infusion_time) ||
      !parse_float(
          request,
          "softInfusionStartPower",
          0.0f,
          100.0f,
          values.soft_infusion_start_power)) {
    return false;
  }

  values.phases.clear();

  // "phases" is optional for compatibility with profiles saved by an older
  // dashboard. The current dashboard already sends it, currently as "".
  if (request->hasParam("phases")) {
    values.phases = request->getParam("phases")->value();

    std::string error;
    if (!valid_phase_spec(values.phases, &error)) {
      ESP_LOGW(
          TAG,
          "Rejected phase specification: %s",
          error.c_str());
      return false;
    }
  }

  return true;
}

inline std::string number(float value) {
  char buffer[32];

  snprintf(
      buffer,
      sizeof(buffer),
      "%.4g",
      static_cast<double>(value));

  return buffer;
}

inline std::string make_profile_json(
    const std::string &id,
    const std::string &name,
    const std::string &created_at,
    const std::string &updated_at,
    const ProfileValues &values) {
  return std::string("{\"id\":\"") +
         json_escape(id) +
         "\",\"name\":\"" +
         json_escape(name) +
         "\",\"version\":2,\"config\":{" +
         "\"brewTarget\":" +
         number(values.brew_target) +
         ",\"targetWeight\":" +
         number(values.target_weight) +
         ",\"shotSeconds\":" +
         number(values.shot_seconds) +
         ",\"prePump\":" +
         number(values.pre_pump) +
         ",\"prePause\":" +
         number(values.pre_pause) +
         ",\"prePower\":" +
         number(values.pre_power) +
         ",\"mainPressure\":" +
         number(values.main_pressure) +
         ",\"endPressure\":" +
         number(values.end_pressure) +
         ",\"softInfusionTime\":" +
         number(values.soft_infusion_time) +
         ",\"softInfusionStartPower\":" +
         number(values.soft_infusion_start_power) +
         ",\"phases\":\"" +
         json_escape(values.phases) +
         "\"},\"createdAt\":" +
         created_at +
         ",\"updatedAt\":" +
         updated_at +
         "}";
}

inline void send_json(
    AsyncWebServerRequest *request,
    int code,
    const std::string &json) {
  auto *response =
      request->beginResponse(
          code,
          "application/json; charset=utf-8",
          json);

  response->addHeader("Cache-Control", "no-store");
  response->addHeader(
      "X-Content-Type-Options",
      "nosniff");

  request->send(response);
}

// -----------------------------------------------------------------------------
// HTTP handler
// -----------------------------------------------------------------------------

class UserProfilesHandler : public AsyncWebHandler {
 public:
  bool canHandle(
      AsyncWebServerRequest *request) const override {
    char buffer[
        AsyncWebServerRequest::URL_BUF_SIZE];

    const std::string url =
        request->url_to(buffer);

    if (request->method() == HTTP_GET)
      return url == "/profiles/index.json";

    return request->method() == HTTP_POST &&
           (url == "/profiles/save" ||
            url.rfind(
                "/profiles/delete/",
                0) == 0);
  }

  void handleRequest(
      AsyncWebServerRequest *request) override {
    char buffer[
        AsyncWebServerRequest::URL_BUF_SIZE];

    const std::string url =
        request->url_to(buffer);

    if (!silvia_archive::mounted &&
        !silvia_archive::mount()) {
      send_json(
          request,
          503,
          "{\"ok\":false,"
          "\"error\":\"filesystem_unavailable\"}");
      return;
    }

    // -----------------------------------------------------------------------
    // GET /profiles/index.json
    // -----------------------------------------------------------------------

    if (request->method() == HTTP_GET) {
      const auto names = list_profile_files();

      std::string json =
          "{\"mounted\":true,"
          "\"count\":0,"
          "\"profiles\":[";

      size_t count = 0;

      for (const auto &name : names) {
        std::string profile;

        const std::string path =
            std::string(
                silvia_archive::BASE_PATH) +
            "/" +
            name;

        if (!read_file(path, profile) ||
            profile.front() != '{' ||
            profile.back() != '}') {
          continue;
        }

        if (count != 0)
          json += ',';

        json += profile;
        ++count;
      }

      json += "]}";

      const std::string marker =
          "\"count\":0";

      const size_t marker_at =
          json.find(marker);

      if (marker_at != std::string::npos) {
        json.replace(
            marker_at,
            marker.size(),
            "\"count\":" +
                std::to_string(count));
      }

      send_json(request, 200, json);
      return;
    }

    // -----------------------------------------------------------------------
    // POST /profiles/delete/<id>
    // -----------------------------------------------------------------------

    if (url.rfind(
            "/profiles/delete/",
            0) == 0) {
      const std::string id =
          url.substr(
              strlen(
                  "/profiles/delete/"));

      if (!valid_id(id)) {
        send_json(
            request,
            400,
            "{\"ok\":false,"
            "\"error\":\"invalid_id\"}");
        return;
      }

      if (std::remove(
              profile_path(id).c_str()) != 0) {
        send_json(
            request,
            404,
            "{\"ok\":false,"
            "\"error\":\"not_found\"}");
        return;
      }

      ESP_LOGI(
          TAG,
          "Deleted user profile %s",
          id.c_str());

      send_json(
          request,
          200,
          "{\"ok\":true}");
      return;
    }

    // -----------------------------------------------------------------------
    // POST /profiles/save
    // -----------------------------------------------------------------------

    if (!request->hasParam("id") ||
        !request->hasParam("name") ||
        !request->hasParam("createdAt") ||
        !request->hasParam("updatedAt")) {
      send_json(
          request,
          400,
          "{\"ok\":false,"
          "\"error\":\"missing_fields\"}");
      return;
    }

    const std::string id =
        request->getParam("id")->value();

    const std::string name =
        request->getParam("name")->value();

    const std::string created_at =
        request->getParam(
            "createdAt")->value();

    const std::string updated_at =
        request->getParam(
            "updatedAt")->value();

    ProfileValues values{};

    if (!valid_id(id) ||
        !valid_name(name) ||
        !valid_timestamp(created_at) ||
        !valid_timestamp(updated_at) ||
        !read_values(request, values)) {
      send_json(
          request,
          400,
          "{\"ok\":false,"
          "\"error\":\"invalid_profile\"}");
      return;
    }

    const std::string path =
        profile_path(id);

    FILE *existing =
        fopen(path.c_str(), "rb");

    const bool profile_exists =
        existing != nullptr;

    if (existing != nullptr)
      fclose(existing);

    if (!profile_exists &&
        list_profile_files().size() >=
            MAX_PROFILES) {
      send_json(
          request,
          409,
          "{\"ok\":false,"
          "\"error\":\"profile_limit\"}");
      return;
    }

    const std::string json =
        make_profile_json(
            id,
            name,
            created_at,
            updated_at,
            values);

    if (!write_atomic(path, json)) {
      send_json(
          request,
          500,
          "{\"ok\":false,"
          "\"error\":\"write_failed\"}");
      return;
    }

    ESP_LOGI(
        TAG,
        "%s user profile %s%s",
        profile_exists
            ? "Updated"
            : "Saved",
        id.c_str(),
        values.phases.empty()
            ? ""
            : " with custom phases");

    send_json(
        request,
        200,
        "{\"ok\":true}");
  }
};

inline UserProfilesHandler user_profiles_handler;
inline bool handler_registered = false;

inline void register_web_handler() {
  if (handler_registered ||
      esphome::web_server_base::
              global_web_server_base ==
          nullptr) {
    return;
  }

  esphome::web_server_base::
      global_web_server_base->
      add_handler(
          &user_profiles_handler);

  handler_registered = true;
}

}  // namespace silvia_profiles
