#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <string>

namespace silvia_analysis {

struct ShotMetadata {
  float dose_g = NAN;
  float target_weight_g = NAN;
  float expected_main_s = NAN;
};

struct ShotAnalysis {
  int score = 0;
  int sensor_confidence = 0;
  bool reliable = false;
  bool channeling_suspected = false;
  std::string quality = "no_data";
  std::string diagnosis = "no_data";
  std::string grind = "unknown";
  float mean_absolute_error_bar = NAN;
  float mean_error_bar = NAN;
  float pressure_instability_bar = NAN;
  float maximum_overshoot_bar = NAN;
  float average_flow_g_s = NAN;
  float flow_variation = NAN;
  float drink_ratio = NAN;
  float main_duration_s = NAN;
  uint32_t sensor_errors = 0;
  uint32_t maximum_sensor_age_ms = 0;
  uint32_t pressure_drop_events = 0;
  size_t working_samples = 0;
};

inline int clamp_percent(int value) {
  return std::max(0, std::min(100, value));
}

template<typename Samples>
ShotAnalysis analyze(const Samples &samples, const ShotMetadata &metadata) {
  ShotAnalysis result;
  if (samples.empty())
    return result;

  const auto &first = samples.front();
  const auto &last = samples.back();
  if (last.xdb_total_errors >= first.xdb_total_errors)
    result.sensor_errors = last.xdb_total_errors - first.xdb_total_errors;

  size_t candidate_samples = 0;
  size_t valid_samples = 0;
  double absolute_error_sum = 0.0;
  double error_sum = 0.0;
  double error_square_sum = 0.0;
  double flow_sum = 0.0;
  double flow_square_sum = 0.0;
  size_t flow_samples = 0;
  bool sensor_fail_seen = false;
  bool have_previous_pressure = false;
  float previous_pressure = 0.0f;
  uint32_t previous_elapsed_ms = 0;
  uint32_t first_working_ms = 0;
  uint32_t last_working_ms = 0;

  result.maximum_overshoot_bar = 0.0f;

  for (const auto &sample : samples) {
    if (sample.startup_state == decltype(sample.startup_state)::SENSOR_FAIL)
      sensor_fail_seen = true;

    if (sample.startup_state == decltype(sample.startup_state)::WAIT_DROP ||
        !std::isfinite(sample.target_bar) || sample.target_bar <= 0.5f)
      continue;

    candidate_samples++;
    result.maximum_sensor_age_ms =
        std::max(result.maximum_sensor_age_ms, sample.sensor_age_ms);

    if (!sample.sensor_ok || !std::isfinite(sample.pressure_bar)) {
      have_previous_pressure = false;
      continue;
    }

    if (valid_samples == 0)
      first_working_ms = sample.elapsed_ms;
    last_working_ms = sample.elapsed_ms;
    valid_samples++;

    const float error = sample.pressure_bar - sample.target_bar;
    absolute_error_sum += std::fabs(error);
    error_sum += error;
    error_square_sum += static_cast<double>(error) * error;
    result.maximum_overshoot_bar =
        std::max(result.maximum_overshoot_bar, error);

    if (std::isfinite(sample.flow_g_s) && sample.flow_g_s > 0.05f &&
        sample.flow_g_s < 12.0f) {
      flow_sum += sample.flow_g_s;
      flow_square_sum += static_cast<double>(sample.flow_g_s) * sample.flow_g_s;
      flow_samples++;
    }

    if (have_previous_pressure) {
      const uint32_t delta_ms = sample.elapsed_ms - previous_elapsed_ms;
      if (delta_ms >= 50U && delta_ms <= 800U &&
          previous_pressure - sample.pressure_bar >= 0.65f)
        result.pressure_drop_events++;
    }
    previous_pressure = sample.pressure_bar;
    previous_elapsed_ms = sample.elapsed_ms;
    have_previous_pressure = true;
  }

  result.working_samples = valid_samples;
  if (candidate_samples == 0 || valid_samples < 3)
    return result;

  result.main_duration_s =
      (last_working_ms - first_working_ms) / 1000.0f;
  result.mean_absolute_error_bar =
      static_cast<float>(absolute_error_sum / valid_samples);
  result.mean_error_bar = static_cast<float>(error_sum / valid_samples);
  const double error_variance =
      std::max(0.0, error_square_sum / valid_samples -
                        (error_sum / valid_samples) * (error_sum / valid_samples));
  result.pressure_instability_bar = static_cast<float>(std::sqrt(error_variance));

  if (flow_samples >= 3) {
    result.average_flow_g_s = static_cast<float>(flow_sum / flow_samples);
    const double flow_variance =
        std::max(0.0, flow_square_sum / flow_samples -
                          (flow_sum / flow_samples) * (flow_sum / flow_samples));
    result.flow_variation = result.average_flow_g_s > 0.05f
                                ? static_cast<float>(std::sqrt(flow_variance) /
                                                     result.average_flow_g_s)
                                : NAN;
  }

  const float final_weight = last.weight_g;
  if (std::isfinite(final_weight) && final_weight > 0.0f &&
      std::isfinite(metadata.target_weight_g) && metadata.target_weight_g > 1.0f)
    result.drink_ratio = final_weight / metadata.target_weight_g;

  const float invalid_ratio =
      1.0f - static_cast<float>(valid_samples) /
                 static_cast<float>(candidate_samples);
  int confidence = 100 - static_cast<int>(std::lround(invalid_ratio * 70.0f));
  confidence -= std::min<int>(40, static_cast<int>(result.sensor_errors) * 4);
  if (result.maximum_sensor_age_ms > 1200U)
    confidence -= 50;
  else if (result.maximum_sensor_age_ms > 800U)
    confidence -= 35;
  else if (result.maximum_sensor_age_ms > 500U)
    confidence -= 20;
  else if (result.maximum_sensor_age_ms > 300U)
    confidence -= 8;
  if (sensor_fail_seen)
    confidence -= 40;
  result.sensor_confidence = clamp_percent(confidence);
  result.reliable = result.sensor_confidence >= 55 && valid_samples >= 8;

  result.channeling_suspected =
      result.reliable && result.main_duration_s >= 5.0f &&
      ((result.pressure_drop_events >= 2U &&
        std::isfinite(result.flow_variation) && result.flow_variation > 0.35f) ||
       (result.pressure_instability_bar > 0.85f &&
        std::isfinite(result.flow_variation) && result.flow_variation > 0.45f));

  int score = 100;
  score -= std::min(30, static_cast<int>(std::lround(
                           std::max(0.0f, result.mean_absolute_error_bar - 0.20f) *
                           18.0f)));
  score -= std::min(22, static_cast<int>(std::lround(
                           std::max(0.0f, result.maximum_overshoot_bar - 0.30f) *
                           12.0f)));
  score -= std::min(18, static_cast<int>(std::lround(
                           std::max(0.0f, result.pressure_instability_bar - 0.25f) *
                           14.0f)));
  if (result.channeling_suspected)
    score -= 12;
  if (std::isfinite(result.drink_ratio)) {
    if (result.drink_ratio < 0.75f)
      score -= 22;
    else if (result.drink_ratio < 0.90f)
      score -= 10;
    else if (result.drink_ratio > 1.18f)
      score -= 14;
    else if (result.drink_ratio > 1.10f)
      score -= 6;
  }
  result.score = clamp_percent(score);

  const bool expected_duration_known =
      std::isfinite(metadata.expected_main_s) && metadata.expected_main_s >= 5.0f;
  const bool too_restrictive =
      std::isfinite(result.drink_ratio) && result.drink_ratio < 0.85f &&
      (!expected_duration_known ||
       result.main_duration_s >= metadata.expected_main_s * 0.80f);
  const bool too_free =
      std::isfinite(result.drink_ratio) && result.drink_ratio >= 0.95f &&
      ((expected_duration_known &&
        result.main_duration_s < metadata.expected_main_s * 0.72f) ||
       (std::isfinite(result.average_flow_g_s) && result.average_flow_g_s > 2.30f));

  if (!result.reliable) {
    result.quality = "sensor_fault";
    result.diagnosis = "sensor_fault";
    result.grind = "unknown";
  } else if (too_restrictive) {
    result.diagnosis = "too_restrictive";
    result.grind = "coarser";
  } else if (too_free) {
    result.diagnosis = "too_free";
    result.grind = "finer";
  } else if (result.maximum_overshoot_bar > 0.80f) {
    result.diagnosis = "overpressure";
    result.grind = "none";
  } else if (result.mean_error_bar < -0.75f) {
    result.diagnosis = "underpressure";
    result.grind = "none";
  } else if (result.channeling_suspected) {
    result.diagnosis = "channeling_suspected";
    result.grind = "check_puck";
  } else if (result.pressure_instability_bar > 0.65f ||
             result.pressure_drop_events >= 2U) {
    result.diagnosis = "unstable";
    result.grind = "none";
  } else {
    result.diagnosis = "normal";
    result.grind = "none";
  }

  if (result.quality != "sensor_fault") {
    if (result.score >= 85)
      result.quality = "good";
    else if (result.score >= 70)
      result.quality = "acceptable";
    else
      result.quality = "poor";
  }
  return result;
}

}  // namespace silvia_analysis
