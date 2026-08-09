#pragma once

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <string>
#include <sys/stat.h>
#include <vector>

#include "esp_littlefs.h"
#include "esphome/core/log.h"
#include "esphome/components/web_server_base/web_server_base.h"
#include "shot_analyzer.h"
#include "shot_diagnostics.h"

namespace silvia_archive {

static constexpr const char *TAG = "shot_archive";
static constexpr const char *BASE_PATH = "/shots";
static constexpr const char *PARTITION = "shots_archive";
static constexpr size_t MAX_LOCAL_SHOTS = 80;

inline bool mounted = false;
inline uint32_t last_saved_started_ms = 0;
inline std::string last_save_status = "not_attempted";
inline std::string pending_profile = "Unknown";
inline float pending_brew_target_c = NAN;
inline float pending_dose_g = NAN;
inline float pending_target_weight_g = NAN;
inline float pending_preinfusion_s = NAN;
inline float pending_pause_s = NAN;
inline float pending_main_s = NAN;
inline uint32_t pending_timestamp = 0;

inline void begin_metadata(const std::string &profile, float brew_target_c,
                           float dose_g, float target_weight_g,
                           float preinfusion_s, float pause_s, float main_s,
                           uint32_t timestamp = 0) {
  pending_profile = profile;
  pending_brew_target_c = brew_target_c;
  pending_dose_g = dose_g;
  pending_target_weight_g = target_weight_g;
  pending_preinfusion_s = preinfusion_s;
  pending_pause_s = pause_s;
  pending_main_s = main_s;
  pending_timestamp = timestamp;
}

inline std::vector<uint32_t> list_ids() {
  std::vector<uint32_t> ids;
  if (!mounted)
    return ids;
  DIR *directory = opendir(BASE_PATH);
  if (directory == nullptr)
    return ids;
  while (dirent *entry = readdir(directory)) {
    unsigned long id = 0;
    char tail = 0;
    if (sscanf(entry->d_name, "shot-%06lu.csv%c", &id, &tail) == 1)
      ids.push_back(static_cast<uint32_t>(id));
  }
  closedir(directory);
  std::sort(ids.begin(), ids.end());
  ids.erase(std::unique(ids.begin(), ids.end()), ids.end());
  return ids;
}

inline std::string path_for(uint32_t id, const char *extension) {
  char path[64];
  snprintf(path, sizeof(path), "%s/shot-%06lu.%s", BASE_PATH,
           static_cast<unsigned long>(id), extension);
  return path;
}

inline void prune_oldest() {
  auto ids = list_ids();
  while (ids.size() >= MAX_LOCAL_SHOTS) {
    const uint32_t id = ids.front();
    std::remove(path_for(id, "csv").c_str());
    std::remove(path_for(id, "json").c_str());
    ids.erase(ids.begin());
  }
}

inline bool mount() {
  if (mounted)
    return true;
  esp_vfs_littlefs_conf_t config{};
  config.base_path = BASE_PATH;
  config.partition_label = PARTITION;
  config.format_if_mount_failed = true;
  config.dont_mount = false;
  const esp_err_t result = esp_vfs_littlefs_register(&config);
  if (result != ESP_OK) {
    ESP_LOGE(TAG, "LittleFS mount failed: %s", esp_err_to_name(result));
    return false;
  }
  mounted = true;
  size_t total = 0;
  size_t used = 0;
  if (esp_littlefs_info(PARTITION, &total, &used) == ESP_OK)
    ESP_LOGI(TAG, "LittleFS ready: %u KiB total, %u KiB used",
             static_cast<unsigned>(total / 1024),
             static_cast<unsigned>(used / 1024));
  return true;
}

inline bool write_atomic(const std::string &path, const std::string &contents) {
  // A shot is written only once, after capture has finished.  Writing the
  // final file directly avoids depending on VFS rename support and still
  // keeps the high-frequency capture path completely in PSRAM.
  FILE *file = fopen(path.c_str(), "wb");
  if (file == nullptr) {
    last_save_status = "open_failed:" + path;
    return false;
  }
  const bool ok = fwrite(contents.data(), 1, contents.size(), file) == contents.size();
  fflush(file);
  fclose(file);
  if (!ok) {
    std::remove(path.c_str());
    last_save_status = "write_failed:" + path;
    return false;
  }
  return true;
}

inline std::string json_number(float value, unsigned precision = 3) {
  if (!std::isfinite(value))
    return "null";
  char buffer[48];
  snprintf(buffer, sizeof(buffer), "%.*f", static_cast<int>(precision), value);
  return buffer;
}

inline std::string make_summary_json(uint32_t id) {
  float maximum_pressure = 0.0f;
  float maximum_overshoot = 0.0f;
  uint32_t maximum_sensor_age = 0;
  bool have_working_sample = false;
  for (const auto &sample : silvia_diag::last_shot) {
    // Residual group pressure during WAIT_DROP is preparation, not an
    // overshoot of the requested extraction profile.
    if (sample.startup_state == silvia_diag::StartupState::WAIT_DROP ||
        sample.target_bar <= 0.1f)
      continue;
    have_working_sample = true;
    maximum_pressure = std::max(maximum_pressure, sample.pressure_bar);
    maximum_overshoot = std::max(maximum_overshoot,
                                 sample.pressure_bar - sample.target_bar);
    maximum_sensor_age = std::max(maximum_sensor_age, sample.sensor_age_ms);
  }
  if (!have_working_sample) {
    maximum_pressure = 0.0f;
    maximum_overshoot = 0.0f;
  }
  const auto &first = silvia_diag::last_shot.front();
  const auto &last = silvia_diag::last_shot.back();
  const uint32_t shot_errors = last.xdb_total_errors - first.xdb_total_errors;
  const silvia_analysis::ShotMetadata analysis_metadata{
      pending_dose_g, pending_target_weight_g, pending_main_s};
  const auto analysis =
      silvia_analysis::analyze(silvia_diag::last_shot, analysis_metadata);
  const std::string timestamp_json = pending_timestamp >= 1577836800U
                                         ? std::to_string(pending_timestamp)
                                         : "null";
  const std::string analysis_mae = json_number(analysis.mean_absolute_error_bar);
  const std::string analysis_mean_error = json_number(analysis.mean_error_bar);
  const std::string analysis_instability = json_number(analysis.pressure_instability_bar);
  const std::string analysis_flow = json_number(analysis.average_flow_g_s);
  const std::string analysis_flow_variation = json_number(analysis.flow_variation);
  const std::string analysis_drink_ratio = json_number(analysis.drink_ratio);
  const std::string analysis_main_duration = json_number(analysis.main_duration_s);
  char csv_name[32];
  snprintf(csv_name, sizeof(csv_name), "shot-%06lu.csv",
           static_cast<unsigned long>(id));

  // Keep the multi-kilobyte summary off the ESPHome/FreeRTOS task stack.
  // std::string owns its payload on the heap; only small string objects and
  // the short CSV filename buffer remain on the stack while a shot is saved.
  std::string json;
  json.reserve(2200);
  json += "{\n";
  json += "  \"id\": " + std::to_string(id) + ",\n";
  json += "  \"timestamp\": " + timestamp_json + ",\n";
  json += "  \"profile\": \"" + pending_profile + "\",\n";
  json += "  \"shot_duration_s\": " + json_number(last.elapsed_ms / 1000.0f) + ",\n";
  json += "  \"brew_temperature_target_c\": " + json_number(pending_brew_target_c, 2) + ",\n";
  json += "  \"coffee\": {\"dose_g\": " + json_number(pending_dose_g, 2) +
          ", \"drink_weight_g\": " + json_number(last.weight_g) +
          ", \"target_weight_g\": " + json_number(pending_target_weight_g, 2) + "},\n";
  json += "  \"preinfusion\": {\"pump_s\": " + json_number(pending_preinfusion_s, 2) +
          ", \"pause_s\": " + json_number(pending_pause_s, 2) + "},\n";
  json += "  \"pressure\": {\"maximum_bar\": " + json_number(maximum_pressure) +
          ", \"maximum_overshoot_bar\": " + json_number(maximum_overshoot) +
          ", \"final_bar\": " + json_number(last.pressure_bar) + "},\n";
  json += "  \"sensor\": {\"max_age_ms\": " + std::to_string(maximum_sensor_age) +
          ", \"shot_errors\": " + std::to_string(shot_errors) + "},\n";
  json += "  \"analysis\": {\n";
  json += "    \"version\": 1,\n";
  json += "    \"quality_score\": " + std::to_string(analysis.score) + ",\n";
  json += "    \"sensor_confidence\": " + std::to_string(analysis.sensor_confidence) + ",\n";
  json += std::string("    \"reliable\": ") + (analysis.reliable ? "true" : "false") + ",\n";
  json += "    \"quality\": \"" + analysis.quality + "\",\n";
  json += "    \"diagnosis\": \"" + analysis.diagnosis + "\",\n";
  json += "    \"suggested_grind\": \"" + analysis.grind + "\",\n";
  json += std::string("    \"channeling_suspected\": ") +
          (analysis.channeling_suspected ? "true" : "false") + ",\n";
  json += "    \"metrics\": {\"mean_absolute_error_bar\": " + analysis_mae +
          ", \"mean_error_bar\": " + analysis_mean_error +
          ", \"pressure_instability_bar\": " + analysis_instability +
          ", \"average_flow_g_s\": " + analysis_flow +
          ", \"flow_variation\": " + analysis_flow_variation +
          ", \"drink_ratio\": " + analysis_drink_ratio +
          ", \"main_duration_s\": " + analysis_main_duration +
          ", \"pressure_drop_events\": " +
          std::to_string(analysis.pressure_drop_events) + "}\n";
  json += "  },\n";
  json += "  \"samples\": " + std::to_string(silvia_diag::last_shot.size()) + ",\n";
  json += "  \"csv\": \"" + std::string(csv_name) + "\"\n";
  json += "}\n";
  return json;
}

inline bool save_current_shot() {
  if (!mounted) {
    last_save_status = "filesystem_not_mounted";
    return false;
  }
  if (!silvia_diag::csv_ready || silvia_diag::last_shot.empty())
    return false;
  if (silvia_diag::shot_started_ms == last_saved_started_ms)
    return false;
  prune_oldest();
  auto ids = list_ids();
  const uint32_t id = ids.empty() ? 1 : ids.back() + 1;
  const std::string csv = silvia_diag::make_csv();
  const std::string csv_path = path_for(id, "csv");
  const std::string json_path = path_for(id, "json");
  if (!write_atomic(csv_path, csv)) {
    ESP_LOGE(TAG, "Failed to save %s", csv_path.c_str());
    return false;
  }
  if (!write_atomic(json_path, make_summary_json(id))) {
    std::remove(csv_path.c_str());
    ESP_LOGE(TAG, "Failed to save %s", json_path.c_str());
    return false;
  }
  last_saved_started_ms = silvia_diag::shot_started_ms;
  last_save_status = "saved:" + std::to_string(id);
  ESP_LOGI(TAG, "Saved local shot %06lu (%u samples)",
           static_cast<unsigned long>(id),
           static_cast<unsigned>(silvia_diag::last_shot.size()));
  return true;
}

inline bool valid_archive_name(const std::string &name) {
  unsigned long id = 0;
  char extension[8]{};
  char tail = 0;
  return sscanf(name.c_str(), "shot-%06lu.%7[a-z]%c", &id, extension, &tail) == 2 &&
         (strcmp(extension, "csv") == 0 || strcmp(extension, "json") == 0);
}

class ArchiveHandler : public AsyncWebHandler {
 public:
  bool canHandle(AsyncWebServerRequest *request) const override {
    char buffer[AsyncWebServerRequest::URL_BUF_SIZE];
    const std::string url = request->url_to(buffer);
    if (request->method() == HTTP_GET)
      return url == "/shots/index.json" || url.rfind("/shots/file/", 0) == 0;
    return request->method() == HTTP_POST && url.rfind("/shots/delete/", 0) == 0;
  }

  void handleRequest(AsyncWebServerRequest *request) override {
    char buffer[AsyncWebServerRequest::URL_BUF_SIZE];
    const std::string url = request->url_to(buffer);
    if (request->method() == HTTP_POST) {
      const std::string value = url.substr(strlen("/shots/delete/"));
      if (value.empty() ||
          !std::all_of(value.begin(), value.end(), [](char c) { return c >= '0' && c <= '9'; })) {
        request->send(400, "application/json", "{\"ok\":false,\"error\":\"invalid_id\"}");
        return;
      }
      const unsigned long parsed = strtoul(value.c_str(), nullptr, 10);
      if (parsed == 0 || parsed > UINT32_MAX) {
        request->send(400, "application/json", "{\"ok\":false,\"error\":\"invalid_id\"}");
        return;
      }
      const uint32_t id = static_cast<uint32_t>(parsed);
      const bool csv_removed = std::remove(path_for(id, "csv").c_str()) == 0;
      const bool json_removed = std::remove(path_for(id, "json").c_str()) == 0;
      if (!csv_removed && !json_removed) {
        request->send(404, "application/json", "{\"ok\":false,\"error\":\"not_found\"}");
        return;
      }
      ESP_LOGI(TAG, "Deleted local shot %06lu", static_cast<unsigned long>(id));
      auto *response = request->beginResponse(200, "application/json", "{\"ok\":true}");
      response->addHeader("Cache-Control", "no-store");
      request->send(response);
      return;
    }
    if (url == "/shots/index.json") {
      const auto ids = list_ids();
      std::string json = "{\"mounted\":" + std::string(mounted ? "true" : "false") +
                         ",\"save_status\":\"" + last_save_status +
                         "\",\"count\":" + std::to_string(ids.size()) +
                         ",\"shots\":[";
      for (size_t i = 0; i < ids.size(); i++) {
        if (i != 0)
          json += ',';
        char item[96];
        snprintf(item, sizeof(item),
                 "{\"id\":%lu,\"csv\":\"shot-%06lu.csv\",\"json\":\"shot-%06lu.json\"}",
                 static_cast<unsigned long>(ids[i]),
                 static_cast<unsigned long>(ids[i]),
                 static_cast<unsigned long>(ids[i]));
        json += item;
      }
      json += "]}";
      auto *response = request->beginResponse(200, "application/json; charset=utf-8", json);
      response->addHeader("Cache-Control", "no-store");
      request->send(response);
      return;
    }
    const std::string name = url.substr(strlen("/shots/file/"));
    if (!valid_archive_name(name)) {
      request->send(400, "text/plain", "Invalid archive name");
      return;
    }
    const std::string path = std::string(BASE_PATH) + "/" + name;
    FILE *file = fopen(path.c_str(), "rb");
    if (file == nullptr) {
      request->send(404, "text/plain", "Archive file not found");
      return;
    }
    fseek(file, 0, SEEK_END);
    const long size = ftell(file);
    rewind(file);
    std::string content;
    if (size > 0)
      content.resize(static_cast<size_t>(size));
    const bool ok = size >= 0 &&
                    (size == 0 || fread(content.data(), 1, content.size(), file) == content.size());
    fclose(file);
    if (!ok) {
      request->send(500, "text/plain", "Archive read failed");
      return;
    }
    const bool csv = name.size() > 4 && name.substr(name.size() - 4) == ".csv";
    auto *response = request->beginResponse(
        200, csv ? "text/csv; charset=utf-8" : "application/json; charset=utf-8", content);
    response->addHeader("Cache-Control", "no-store");
    request->send(response);
  }
};

inline ArchiveHandler archive_handler;
inline bool handler_registered = false;

inline void register_web_handler() {
  if (handler_registered || esphome::web_server_base::global_web_server_base == nullptr)
    return;
  esphome::web_server_base::global_web_server_base->add_handler(&archive_handler);
  handler_registered = true;
}

}  // namespace silvia_archive
