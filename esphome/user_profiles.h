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

namespace silvia_profiles {

static constexpr const char *TAG = "user_profiles";
static constexpr const char *FILE_PREFIX = "profile-";
static constexpr const char *FILE_SUFFIX = ".json";
static constexpr size_t MAX_PROFILES = 30;

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
};

inline std::string json_escape(const std::string &value) {
  std::string result;
  result.reserve(value.size() + 8);
  for (const unsigned char c : value) {
    switch (c) {
      case '\"': result += "\\\""; break;
      case '\\': result += "\\\\"; break;
      case '\b': result += "\\b"; break;
      case '\f': result += "\\f"; break;
      case '\n': result += "\\n"; break;
      case '\r': result += "\\r"; break;
      case '\t': result += "\\t"; break;
      default:
        if (c < 0x20) {
          char escaped[7];
          snprintf(escaped, sizeof(escaped), "\\u%04x", static_cast<unsigned>(c));
          result += escaped;
        } else {
          result.push_back(static_cast<char>(c));
        }
    }
  }
  return result;
}

inline bool valid_id(const std::string &id) {
  return !id.empty() && id.size() <= 64 &&
         std::all_of(id.begin(), id.end(), [](unsigned char c) {
           return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                  (c >= '0' && c <= '9') || c == '-' || c == '_';
         });
}

inline bool valid_name(const std::string &name) {
  if (name.empty() || name.size() > 128)
    return false;
  return std::none_of(name.begin(), name.end(), [](unsigned char c) {
    return c < 0x20 || c == 0x7f;
  });
}

inline bool valid_timestamp(const std::string &value) {
  return !value.empty() && value.size() <= 16 &&
         std::all_of(value.begin(), value.end(), [](unsigned char c) {
           return c >= '0' && c <= '9';
         });
}

inline std::string profile_path(const std::string &id) {
  return std::string(silvia_archive::BASE_PATH) + "/" + FILE_PREFIX + id + FILE_SUFFIX;
}

inline bool is_profile_file(const std::string &name) {
  const size_t prefix_length = strlen(FILE_PREFIX);
  const size_t suffix_length = strlen(FILE_SUFFIX);
  return name.size() > prefix_length + suffix_length &&
         name.compare(0, prefix_length, FILE_PREFIX) == 0 &&
         name.compare(name.size() - suffix_length, suffix_length, FILE_SUFFIX) == 0;
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

inline bool read_file(const std::string &path, std::string &contents) {
  FILE *file = fopen(path.c_str(), "rb");
  if (file == nullptr)
    return false;
  if (fseek(file, 0, SEEK_END) != 0) {
    fclose(file);
    return false;
  }
  const long size = ftell(file);
  if (size < 2 || size > 8192) {
    fclose(file);
    return false;
  }
  rewind(file);
  contents.resize(static_cast<size_t>(size));
  const bool ok = fread(contents.data(), 1, contents.size(), file) == contents.size();
  fclose(file);
  return ok;
}

inline bool write_atomic(const std::string &path, const std::string &contents) {
  const std::string temporary = path + ".tmp";
  FILE *file = fopen(temporary.c_str(), "wb");
  if (file == nullptr)
    return false;
  const bool written = fwrite(contents.data(), 1, contents.size(), file) == contents.size();
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

inline bool parse_float(AsyncWebServerRequest *request, const char *name,
                        float minimum, float maximum, float &result) {
  if (!request->hasParam(name))
    return false;
  const std::string value = request->getParam(name)->value();
  char *tail = nullptr;
  result = strtof(value.c_str(), &tail);
  return tail != value.c_str() && tail != nullptr && *tail == '\0' &&
         std::isfinite(result) && result >= minimum && result <= maximum;
}

inline bool read_values(AsyncWebServerRequest *request, ProfileValues &values) {
  return parse_float(request, "brewTarget", 80.0f, 105.0f, values.brew_target) &&
         parse_float(request, "targetWeight", 5.0f, 100.0f, values.target_weight) &&
         parse_float(request, "shotSeconds", 1.0f, 90.0f, values.shot_seconds) &&
         parse_float(request, "prePump", 0.0f, 10.0f, values.pre_pump) &&
         parse_float(request, "prePause", 0.0f, 20.0f, values.pre_pause) &&
         parse_float(request, "prePower", 0.0f, 12.0f, values.pre_power) &&
         parse_float(request, "mainPressure", 0.0f, 12.0f, values.main_pressure) &&
         parse_float(request, "endPressure", 0.0f, 12.0f, values.end_pressure) &&
         parse_float(request, "softInfusionTime", 0.0f, 5.0f, values.soft_infusion_time) &&
         parse_float(request, "softInfusionStartPower", 5.0f, 60.0f,
                     values.soft_infusion_start_power);
}

inline std::string number(float value) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%.4g", static_cast<double>(value));
  return buffer;
}

inline std::string make_profile_json(const std::string &id, const std::string &name,
                                     const std::string &created_at,
                                     const std::string &updated_at,
                                     const ProfileValues &values) {
  return std::string("{\"id\":\"") + json_escape(id) +
         "\",\"name\":\"" + json_escape(name) +
         "\",\"config\":{" +
         "\"brewTarget\":" + number(values.brew_target) +
         ",\"targetWeight\":" + number(values.target_weight) +
         ",\"shotSeconds\":" + number(values.shot_seconds) +
         ",\"prePump\":" + number(values.pre_pump) +
         ",\"prePause\":" + number(values.pre_pause) +
         ",\"prePower\":" + number(values.pre_power) +
         ",\"mainPressure\":" + number(values.main_pressure) +
         ",\"endPressure\":" + number(values.end_pressure) +
         ",\"softInfusionTime\":" + number(values.soft_infusion_time) +
         ",\"softInfusionStartPower\":" + number(values.soft_infusion_start_power) +
         "},\"createdAt\":" + created_at + ",\"updatedAt\":" + updated_at + "}";
}

inline void send_json(AsyncWebServerRequest *request, int code, const std::string &json) {
  auto *response = request->beginResponse(code, "application/json; charset=utf-8", json);
  response->addHeader("Cache-Control", "no-store");
  response->addHeader("X-Content-Type-Options", "nosniff");
  request->send(response);
}

class UserProfilesHandler : public AsyncWebHandler {
 public:
  bool canHandle(AsyncWebServerRequest *request) const override {
    char buffer[AsyncWebServerRequest::URL_BUF_SIZE];
    const std::string url = request->url_to(buffer);
    if (request->method() == HTTP_GET)
      return url == "/profiles/index.json";
    return request->method() == HTTP_POST &&
           (url == "/profiles/save" || url.rfind("/profiles/delete/", 0) == 0);
  }

  void handleRequest(AsyncWebServerRequest *request) override {
    char buffer[AsyncWebServerRequest::URL_BUF_SIZE];
    const std::string url = request->url_to(buffer);
    if (!silvia_archive::mounted && !silvia_archive::mount()) {
      send_json(request, 503, "{\"ok\":false,\"error\":\"filesystem_unavailable\"}");
      return;
    }

    if (request->method() == HTTP_GET) {
      const auto names = list_profile_files();
      std::string json = "{\"mounted\":true,\"count\":0,\"profiles\":[";
      size_t count = 0;
      for (const auto &name : names) {
        std::string profile;
        const std::string path = std::string(silvia_archive::BASE_PATH) + "/" + name;
        if (!read_file(path, profile) || profile.front() != '{' || profile.back() != '}')
          continue;
        if (count != 0)
          json += ',';
        json += profile;
        count++;
      }
      json += "]}";
      const std::string marker = "\"count\":0";
      const size_t marker_at = json.find(marker);
      if (marker_at != std::string::npos)
        json.replace(marker_at, marker.size(), "\"count\":" + std::to_string(count));
      send_json(request, 200, json);
      return;
    }

    if (url.rfind("/profiles/delete/", 0) == 0) {
      const std::string id = url.substr(strlen("/profiles/delete/"));
      if (!valid_id(id)) {
        send_json(request, 400, "{\"ok\":false,\"error\":\"invalid_id\"}");
        return;
      }
      if (std::remove(profile_path(id).c_str()) != 0) {
        send_json(request, 404, "{\"ok\":false,\"error\":\"not_found\"}");
        return;
      }
      ESP_LOGI(TAG, "Deleted user profile %s", id.c_str());
      send_json(request, 200, "{\"ok\":true}");
      return;
    }

    if (!request->hasParam("id") || !request->hasParam("name") ||
        !request->hasParam("createdAt") || !request->hasParam("updatedAt")) {
      send_json(request, 400, "{\"ok\":false,\"error\":\"missing_fields\"}");
      return;
    }
    const std::string id = request->getParam("id")->value();
    const std::string name = request->getParam("name")->value();
    const std::string created_at = request->getParam("createdAt")->value();
    const std::string updated_at = request->getParam("updatedAt")->value();
    ProfileValues values{};
    if (!valid_id(id) || !valid_name(name) || !valid_timestamp(created_at) ||
        !valid_timestamp(updated_at) || !read_values(request, values)) {
      send_json(request, 400, "{\"ok\":false,\"error\":\"invalid_profile\"}");
      return;
    }

    const std::string path = profile_path(id);
    FILE *existing = fopen(path.c_str(), "rb");
    const bool profile_exists = existing != nullptr;
    if (existing != nullptr)
      fclose(existing);
    if (!profile_exists && list_profile_files().size() >= MAX_PROFILES) {
      send_json(request, 409, "{\"ok\":false,\"error\":\"profile_limit\"}");
      return;
    }

    const std::string json = make_profile_json(id, name, created_at, updated_at, values);
    if (!write_atomic(path, json)) {
      send_json(request, 500, "{\"ok\":false,\"error\":\"write_failed\"}");
      return;
    }
    ESP_LOGI(TAG, "%s user profile %s", profile_exists ? "Updated" : "Saved", id.c_str());
    send_json(request, 200, "{\"ok\":true}");
  }
};

inline UserProfilesHandler user_profiles_handler;
inline bool handler_registered = false;

inline void register_web_handler() {
  if (handler_registered || esphome::web_server_base::global_web_server_base == nullptr)
    return;
  esphome::web_server_base::global_web_server_base->add_handler(&user_profiles_handler);
  handler_registered = true;
}

}  // namespace silvia_profiles
