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
static constexpr size_t MIN_FREE_BYTES = 64U * 1024U;
static constexpr const char *TRAINING_SETTINGS_PATH = "/shots/training.conf";

inline bool mounted = false;
inline bool training_collection_enabled = true;
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
inline bool pending_training_collection = false;

inline void begin_metadata(const std::string &profile, float brew_target_c,
                           float dose_g, float target_weight_g,
                           float preinfusion_s, float pause_s, float main_s,
                           uint32_t timestamp = 0,
                           bool training_collection = false) {
  pending_profile = profile;
  pending_brew_target_c = brew_target_c;
  pending_dose_g = dose_g;
  pending_target_weight_g = target_weight_g;
  pending_preinfusion_s = preinfusion_s;
  pending_pause_s = pause_s;
  pending_main_s = main_s;
  pending_timestamp = timestamp;
  pending_training_collection = training_collection;
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

inline bool file_exists(const std::string &path) {
  struct stat info {};
  return stat(path.c_str(), &info) == 0;
}

inline bool is_pinned(uint32_t id) {
  return file_exists(path_for(id, "pin"));
}

inline void remove_shot_files(uint32_t id) {
  std::remove(path_for(id, "csv").c_str());
  std::remove(path_for(id, "json").c_str());
  std::remove(path_for(id, "train").c_str());
  std::remove(path_for(id, "pin").c_str());
}

inline bool remove_oldest_unpinned() {
  const auto ids = list_ids();
  for (const uint32_t id : ids) {
    if (is_pinned(id))
      continue;
    remove_shot_files(id);
    ESP_LOGI(TAG, "Pruned local shot %06lu", static_cast<unsigned long>(id));
    return true;
  }
  return false;
}

inline bool ensure_capacity(size_t required_bytes) {
  while (list_ids().size() >= MAX_LOCAL_SHOTS) {
    if (!remove_oldest_unpinned())
      return false;
  }
  while (true) {
    size_t total = 0;
    size_t used = 0;
    if (esp_littlefs_info(PARTITION, &total, &used) != ESP_OK)
      return true;
    const size_t free_bytes = total > used ? total - used : 0;
    if (free_bytes >= required_bytes + MIN_FREE_BYTES)
      return true;
    if (!remove_oldest_unpinned())
      return false;
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
  FILE *settings = fopen(TRAINING_SETTINGS_PATH, "rb");
  if (settings != nullptr) {
    const int value = fgetc(settings);
    fclose(settings);
    training_collection_enabled = value != '0';
  }
  size_t total = 0;
  size_t used = 0;
  if (esp_littlefs_info(PARTITION, &total, &used) == ESP_OK)
    ESP_LOGI(TAG, "LittleFS ready: %u KiB total, %u KiB used",
             static_cast<unsigned>(total / 1024),
             static_cast<unsigned>(used / 1024));
  return true;
}

template<typename Allocator>
inline bool write_atomic(
    const std::string &path,
    const std::basic_string<char, std::char_traits<char>, Allocator> &contents) {
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

inline bool write_atomic(const std::string &path, const char *contents) {
  return write_atomic(path, std::string(contents));
}

inline std::string json_number(float value, unsigned precision = 3) {
  if (!std::isfinite(value))
    return "null";
  char buffer[48];
  snprintf(buffer, sizeof(buffer), "%.*f", static_cast<int>(precision), value);
  return buffer;
}

inline std::string make_training_json(
    uint32_t id,
    const silvia_analysis::TrainingAssessment &training,
    size_t csv_bytes) {
  const std::string timestamp_json = pending_timestamp >= 1577836800U
                                         ? std::to_string(pending_timestamp)
                                         : "null";
  std::string json;
  json.reserve(900);
  json += "{\"version\":1";
  json += ",\"id\":" + std::to_string(id);
  json += ",\"timestamp\":" + timestamp_json;
  json += ",\"profile\":\"" + pending_profile + "\"";
  json += std::string(",\"collected\":") +
          (pending_training_collection ? "true" : "false");
  json += ",\"status\":\"" + training.status + "\"";
  json += std::string(",\"eligible\":") +
          (training.eligible ? "true" : "false");
  json += ",\"issues\":" + std::to_string(training.issues);
  json += ",\"samples\":" + std::to_string(training.total_samples);
  json += ",\"working_samples\":" +
          std::to_string(training.working_samples);
  json += ",\"valid_samples\":" +
          std::to_string(training.valid_working_samples);
  json += ",\"invalid_samples\":" +
          std::to_string(training.invalid_working_samples);
  json += ",\"usable_windows\":" +
          std::to_string(training.usable_windows);
  json += ",\"sensor_errors\":" +
          std::to_string(training.sensor_errors);
  json += ",\"max_sensor_age_ms\":" +
          std::to_string(training.maximum_sensor_age_ms);
  json += ",\"max_consecutive_errors\":" +
          std::to_string(training.maximum_consecutive_errors);
  json += ",\"max_sample_gap_ms\":" +
          std::to_string(training.maximum_sample_gap_ms);
  json += ",\"timeline_gaps\":" +
          std::to_string(training.timeline_gap_events);
  json += ",\"pressure_jumps\":" +
          std::to_string(training.pressure_jump_events);
  json += ",\"phase_mask\":" + std::to_string(training.phase_mask);
  // Profile tracking accuracy — the figure controller work is judged by.
  json += ",\"tracking_samples\":" +
          std::to_string(training.tracking_samples);
  json += ",\"tracking_rmse_bar\":" +
          json_number(training.tracking_rmse_bar);
  json += ",\"tracking_max_abs_error_bar\":" +
          json_number(training.tracking_max_abs_error_bar);
  json += ",\"tracking_mean_error_bar\":" +
          json_number(training.tracking_mean_error_bar);
  json += ",\"csv_bytes\":" + std::to_string(csv_bytes);
  json += "}";
  return json;
}

inline std::string make_summary_json(
    uint32_t id,
    const silvia_analysis::TrainingAssessment &training) {
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
  json += "  \"training\": {\"version\": 1, \"collected\": " +
          std::string(pending_training_collection ? "true" : "false") +
          ", \"status\": \"" + training.status +
          "\", \"eligible\": " +
          std::string(training.eligible ? "true" : "false") +
          ", \"issues\": " + std::to_string(training.issues) +
          ", \"usable_windows\": " +
          std::to_string(training.usable_windows) +
          ", \"valid_samples\": " +
          std::to_string(training.valid_working_samples) +
          ", \"invalid_samples\": " +
          std::to_string(training.invalid_working_samples) + "},\n";
  json += "  \"analysis\": {\n";
  // Version 2: tracking metrics are scored over the brew phase only, starting
  // once the pressure first reached the profile. Numbers from version 1 were
  // averaged over preinfusion and the initial climb too, so the dashboard must
  // not judge them with the current thresholds.
  json += "    \"version\": 2,\n";
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
  // "Is there anything to save?" is checked before "can we save?". This
  // function is polled once per second and is also called from power-off and
  // shot-end paths, some of which can run before the filesystem is mounted.
  // Checking mount state first made those idle calls latch a
  // "filesystem_not_mounted" status that then stayed visible on the
  // diagnostics page for the rest of the session, long after the mount
  // succeeded and shots were being stored normally.
  if (!silvia_diag::csv_ready || silvia_diag::last_shot.empty())
    return false;
  if (silvia_diag::shot_started_ms == last_saved_started_ms)
    return false;
  if (!mounted) {
    last_save_status = "filesystem_not_mounted";
    return false;
  }
  auto ids = list_ids();
  const uint32_t id = ids.empty() ? 1 : ids.back() + 1;
  const auto training =
      silvia_analysis::assess_for_training(silvia_diag::last_shot);
  const auto csv = silvia_diag::make_csv();
  const std::string summary = make_summary_json(id, training);
  const std::string training_json = make_training_json(id, training, csv.size());
  const size_t required_bytes =
      csv.size() + summary.size() + training_json.size() + 4096U;
  if (!ensure_capacity(required_bytes)) {
    last_save_status = "archive_full_pinned";
    ESP_LOGE(TAG, "Cannot save shot: archive is full and all old shots are pinned");
    return false;
  }
  const std::string csv_path = path_for(id, "csv");
  const std::string json_path = path_for(id, "json");
  const std::string training_path = path_for(id, "train");
  if (!write_atomic(csv_path, csv)) {
    ESP_LOGE(TAG, "Failed to save %s", csv_path.c_str());
    return false;
  }
  if (!write_atomic(json_path, summary)) {
    std::remove(csv_path.c_str());
    ESP_LOGE(TAG, "Failed to save %s", json_path.c_str());
    return false;
  }
  if (!write_atomic(training_path, training_json)) {
    std::remove(csv_path.c_str());
    std::remove(json_path.c_str());
    ESP_LOGE(TAG, "Failed to save %s", training_path.c_str());
    return false;
  }
  last_saved_started_ms = silvia_diag::shot_started_ms;
  last_save_status = "saved:" + std::to_string(id) + ":" + training.status;
  ESP_LOGI(TAG, "Saved local shot %06lu (%u samples, training=%s, collected=%s)",
           static_cast<unsigned long>(id),
           static_cast<unsigned>(silvia_diag::last_shot.size()),
           training.status.c_str(),
           pending_training_collection ? "yes" : "no");
  return true;
}

inline bool read_file(const std::string &path, std::string &content,
                      size_t maximum_bytes = 4096U) {
  FILE *file = fopen(path.c_str(), "rb");
  if (file == nullptr)
    return false;
  fseek(file, 0, SEEK_END);
  const long size = ftell(file);
  rewind(file);
  if (size < 0 || static_cast<size_t>(size) > maximum_bytes) {
    fclose(file);
    return false;
  }
  content.resize(static_cast<size_t>(size));
  const bool ok = size == 0 ||
                  fread(content.data(), 1, content.size(), file) == content.size();
  fclose(file);
  if (!ok)
    content.clear();
  return ok;
}

inline bool set_training_collection(bool enabled) {
  if (!mounted)
    return false;
  if (!write_atomic(TRAINING_SETTINGS_PATH, enabled ? "1\n" : "0\n"))
    return false;
  training_collection_enabled = enabled;
  return true;
}

inline bool parse_id_after(const std::string &url, const char *prefix,
                           uint32_t &id) {
  if (url.rfind(prefix, 0) != 0)
    return false;
  const std::string value = url.substr(strlen(prefix));
  if (value.empty() ||
      !std::all_of(value.begin(), value.end(),
                   [](char c) { return c >= '0' && c <= '9'; }))
    return false;
  const unsigned long parsed = strtoul(value.c_str(), nullptr, 10);
  if (parsed == 0 || parsed > UINT32_MAX)
    return false;
  id = static_cast<uint32_t>(parsed);
  return true;
}

inline bool valid_archive_name(const std::string &name) {
  unsigned long id = 0;
  char extension[8]{};
  char tail = 0;
  return sscanf(name.c_str(), "shot-%06lu.%7[a-z]%c", &id, extension, &tail) == 2 &&
         (strcmp(extension, "csv") == 0 || strcmp(extension, "json") == 0);
}

inline void stream_archive_file(AsyncWebServerRequest *request, FILE *file,
                                const char *content_type) {
  static constexpr size_t STREAM_CHUNK_SIZE = 1024;
  char *chunk = static_cast<char *>(malloc(STREAM_CHUNK_SIZE));
  if (chunk == nullptr) {
    fclose(file);
    ESP_LOGW(TAG, "Could not allocate archive streaming buffer");
    request->send(500, "text/plain", "Archive streaming buffer unavailable");
    return;
  }

  httpd_resp_set_status(*request, HTTPD_200);
  httpd_resp_set_type(*request, content_type);
  httpd_resp_set_hdr(*request, "Cache-Control", "no-store");
  httpd_resp_set_hdr(*request, "Accept-Ranges", "none");

  // Keep archive payloads out of std::string and the large transfer buffer off
  // the deliberately small ESP-IDF HTTP task stack.
  bool read_ok = true;
  bool send_ok = true;
  while (send_ok) {
    const size_t count = fread(chunk, 1, STREAM_CHUNK_SIZE, file);
    if (count > 0 && httpd_resp_send_chunk(*request, chunk, count) != ESP_OK) {
      send_ok = false;
      break;
    }
    if (count < STREAM_CHUNK_SIZE) {
      read_ok = !ferror(file);
      break;
    }
  }
  fclose(file);
  free(chunk);

  if (read_ok && send_ok) {
    httpd_resp_send_chunk(*request, nullptr, 0);
  } else {
    ESP_LOGW(TAG, "Archive streaming failed (read_ok=%s, send_ok=%s)",
             read_ok ? "true" : "false", send_ok ? "true" : "false");
  }
}

class ArchiveHandler : public AsyncWebHandler {
 public:
  bool canHandle(AsyncWebServerRequest *request) const override {
    char buffer[AsyncWebServerRequest::URL_BUF_SIZE];
    const std::string url = request->url_to(buffer);
    if (request->method() == HTTP_GET)
      return url == "/shots/index.json" ||
             url == "/training/index.json" ||
             url.rfind("/shots/file/", 0) == 0;
    return request->method() == HTTP_POST &&
           (url == "/training/collection/on" ||
            url == "/training/collection/off" ||
            url.rfind("/training/pin/", 0) == 0 ||
            url.rfind("/training/unpin/", 0) == 0 ||
            url.rfind("/shots/delete/", 0) == 0);
  }

  void handleRequest(AsyncWebServerRequest *request) override {
    char buffer[AsyncWebServerRequest::URL_BUF_SIZE];
    const std::string url = request->url_to(buffer);
    if (request->method() == HTTP_POST) {
      if (url == "/training/collection/on" ||
          url == "/training/collection/off") {
        const bool enabled = url == "/training/collection/on";
        if (!set_training_collection(enabled)) {
          request->send(500, "application/json",
                        "{\"ok\":false,\"error\":\"settings_write_failed\"}");
          return;
        }
        const std::string json =
            std::string("{\"ok\":true,\"collection_enabled\":") +
            (enabled ? "true}" : "false}");
        auto *response = request->beginResponse(200, "application/json", json);
        response->addHeader("Cache-Control", "no-store");
        request->send(response);
        return;
      }

      uint32_t id = 0;
      const bool pin = parse_id_after(url, "/training/pin/", id);
      const bool unpin = !pin && parse_id_after(url, "/training/unpin/", id);
      if (pin || unpin) {
        if (!file_exists(path_for(id, "csv"))) {
          request->send(404, "application/json",
                        "{\"ok\":false,\"error\":\"not_found\"}");
          return;
        }
        const bool ok = pin
                            ? write_atomic(path_for(id, "pin"), "1\n")
                            : (std::remove(path_for(id, "pin").c_str()) == 0 ||
                               !file_exists(path_for(id, "pin")));
        if (!ok) {
          request->send(500, "application/json",
                        "{\"ok\":false,\"error\":\"pin_write_failed\"}");
          return;
        }
        const std::string json =
            std::string("{\"ok\":true,\"id\":") + std::to_string(id) +
            ",\"pinned\":" + (pin ? "true}" : "false}");
        auto *response = request->beginResponse(200, "application/json", json);
        response->addHeader("Cache-Control", "no-store");
        request->send(response);
        return;
      }

      if (!parse_id_after(url, "/shots/delete/", id)) {
        request->send(400, "application/json", "{\"ok\":false,\"error\":\"invalid_id\"}");
        return;
      }
      const bool existed = file_exists(path_for(id, "csv")) ||
                           file_exists(path_for(id, "json"));
      if (!existed) {
        request->send(404, "application/json", "{\"ok\":false,\"error\":\"not_found\"}");
        return;
      }
      remove_shot_files(id);
      ESP_LOGI(TAG, "Deleted local shot %06lu", static_cast<unsigned long>(id));
      auto *response = request->beginResponse(200, "application/json", "{\"ok\":true}");
      response->addHeader("Cache-Control", "no-store");
      request->send(response);
      return;
    }
    if (url == "/training/index.json") {
      const auto ids = list_ids();
      size_t total = 0;
      size_t used = 0;
      esp_littlefs_info(PARTITION, &total, &used);
      std::string json =
          "{\"version\":1,\"collection_enabled\":" +
          std::string(training_collection_enabled ? "true" : "false") +
          ",\"storage\":{\"total_bytes\":" + std::to_string(total) +
          ",\"used_bytes\":" + std::to_string(used) +
          ",\"free_bytes\":" + std::to_string(total > used ? total - used : 0) +
          "},\"records\":[";
      bool first_record = true;
      for (const uint32_t id : ids) {
        std::string training;
        if (!read_file(path_for(id, "train"), training, 2048U) || training.empty())
          continue;
        if (!first_record)
          json += ',';
        first_record = false;
        json += "{\"pinned\":";
        json += is_pinned(id) ? "true" : "false";
        json += ",\"record\":";
        json += training;
        json += '}';
      }
      json += "]}";
      auto *response = request->beginResponse(
          200, "application/json; charset=utf-8", json);
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
    const bool csv = name.size() > 4 && name.substr(name.size() - 4) == ".csv";
    stream_archive_file(
        request, file,
        csv ? "text/csv; charset=utf-8" : "application/json; charset=utf-8");
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
