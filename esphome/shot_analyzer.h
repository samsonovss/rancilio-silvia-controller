#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <string>

#include "shot_profiles.h"

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

enum TrainingIssue : uint32_t {
  TRAINING_ISSUE_NONE = 0,
  TRAINING_ISSUE_NO_PRESSURE_PHASE = 1U << 0,
  TRAINING_ISSUE_TOO_SHORT = 1U << 1,
  TRAINING_ISSUE_SENSOR_ERROR = 1U << 2,
  TRAINING_ISSUE_SENSOR_INVALID = 1U << 3,
  TRAINING_ISSUE_SENSOR_STALE = 1U << 4,
  TRAINING_ISSUE_SENSOR_FAIL = 1U << 5,
  TRAINING_ISSUE_CONSECUTIVE_ERRORS = 1U << 6,
  TRAINING_ISSUE_TIMELINE_GAP = 1U << 7,
  TRAINING_ISSUE_PRESSURE_JUMP = 1U << 8,
  TRAINING_ISSUE_NO_USABLE_WINDOWS = 1U << 9,
};

struct TrainingAssessment {
  // A 2 s input history and a 1 s prediction horizon at the 10 Hz capture
  // rate.  The computer-side trainer may later choose shorter horizons, but
  // every exported window is guaranteed to cover the longest planned
  // prediction.  These counts were doubled together with the capture rate so
  // the covered duration stayed the same.
  static constexpr size_t HISTORY_SAMPLES = 20;
  static constexpr size_t HORIZON_SAMPLES = 10;

  // Nominal capture period. Every threshold below that talks about time or
  // about a per-sample change is derived from it, so that changing the capture
  // rate again cannot silently loosen the acceptance criteria.
  static constexpr uint32_t NOMINAL_PERIOD_MS = 100;
  // At most one skipped tick, plus jitter. A training window assumes a uniform
  // time step; a larger hole would make a fixed-length window cover an
  // unpredictable amount of real time.
  static constexpr uint32_t MAX_GAP_MS = 250;
  // Five seconds of usable pressure phase.
  static constexpr size_t MIN_VALID_SAMPLES = 50;
  // 12.5 bar/s, expressed per sample. The driver already rejects anything
  // faster than 15 bar/s upward, so this catches what slipped past it.
  static constexpr float MAX_STEP_BAR_PER_S = 12.5f;

  std::string status = "rejected";
  bool eligible = false;
  uint32_t issues = TRAINING_ISSUE_NONE;
  size_t total_samples = 0;
  size_t working_samples = 0;
  size_t valid_working_samples = 0;
  size_t invalid_working_samples = 0;
  size_t usable_windows = 0;
  uint32_t sensor_errors = 0;
  uint32_t maximum_sensor_age_ms = 0;
  uint32_t maximum_consecutive_errors = 0;
  uint32_t maximum_sample_gap_ms = 0;
  uint32_t timeline_gap_events = 0;
  uint32_t pressure_jump_events = 0;
  uint32_t phase_mask = 0;

  // How closely the measured pressure followed the profile. This is the
  // scoreboard for controller work: it is the quantity a learned feed-forward
  // is supposed to reduce, so it must be comparable across shots.
  //
  // Only samples where the PI loop was actually in command are counted. The
  // soft-start ramp and the startup search are excluded because there the
  // controller is deliberately not tracking the curve, and including them
  // would reward a controller for slamming the pump early.
  //
  // Caveat worth remembering when reading these numbers: during channeling the
  // puck itself changes and the pressure genuinely cannot follow. Such a shot
  // shows a large error through no fault of the controller, which is why
  // flow_variation and pressure_drop_events must be read alongside it.
  size_t tracking_samples = 0;
  float tracking_rmse_bar = 0.0f;
  float tracking_max_abs_error_bar = 0.0f;
  float tracking_mean_error_bar = 0.0f;  // signed: negative means undershoot
};

inline int clamp_percent(int value) {
  return std::max(0, std::min(100, value));
}

template<typename Samples>
TrainingAssessment assess_for_training(const Samples &samples) {
  TrainingAssessment result;
  result.total_samples = samples.size();
  if (samples.empty()) {
    result.issues = TRAINING_ISSUE_NO_PRESSURE_PHASE |
                    TRAINING_ISSUE_TOO_SHORT |
                    TRAINING_ISSUE_NO_USABLE_WINDOWS;
    return result;
  }

  const auto &first = samples.front();
  const auto &last = samples.back();
  if (last.xdb_total_errors >= first.xdb_total_errors)
    result.sensor_errors = last.xdb_total_errors - first.xdb_total_errors;

  bool have_previous_sample = false;
  bool have_previous_pressure = false;
  uint32_t previous_elapsed_ms = 0;
  float previous_pressure_bar = 0.0f;
  size_t contiguous_valid_samples = 0;
  double tracking_error_sum = 0.0;
  double tracking_square_sum = 0.0;
  bool tracking_started = false;

  const auto finish_valid_run = [&]() {
    const size_t required = TrainingAssessment::HISTORY_SAMPLES +
                            TrainingAssessment::HORIZON_SAMPLES;
    if (contiguous_valid_samples >= required)
      result.usable_windows += contiguous_valid_samples - required + 1;
    contiguous_valid_samples = 0;
  };

  for (const auto &sample : samples) {
    const bool pressure_phase =
        sample.startup_state != decltype(sample.startup_state)::WAIT_DROP &&
        std::isfinite(sample.target_bar) && sample.target_bar > 0.5f;

    if (sample.startup_state == decltype(sample.startup_state)::SENSOR_FAIL)
      result.issues |= TRAINING_ISSUE_SENSOR_FAIL;

    result.maximum_consecutive_errors =
        std::max(result.maximum_consecutive_errors,
                 sample.xdb_consecutive_errors);

    if (!pressure_phase) {
      finish_valid_run();
      have_previous_sample = false;
      have_previous_pressure = false;
      continue;
    }

    result.working_samples++;
    if (sample.phase < 32U)
      result.phase_mask |= 1U << sample.phase;
    result.maximum_sensor_age_ms =
        std::max(result.maximum_sensor_age_ms, sample.sensor_age_ms);

    uint32_t gap_ms = 0;
    if (have_previous_sample) {
      gap_ms = sample.elapsed_ms >= previous_elapsed_ms
                   ? sample.elapsed_ms - previous_elapsed_ms
                   : UINT32_MAX;
      result.maximum_sample_gap_ms =
          std::max(result.maximum_sample_gap_ms, gap_ms);
      if (gap_ms > TrainingAssessment::MAX_GAP_MS)
        result.timeline_gap_events++;
    }
    have_previous_sample = true;
    previous_elapsed_ms = sample.elapsed_ms;

    const bool valid = sample.sensor_ok &&
                       std::isfinite(sample.pressure_bar) &&
                       sample.pressure_bar >= -0.5f &&
                       sample.pressure_bar <= 13.0f &&
                       sample.sensor_age_ms <= 300U &&
                       sample.xdb_consecutive_errors == 0U &&
                       gap_ms != UINT32_MAX &&
                       (!have_previous_pressure ||
                        gap_ms <= TrainingAssessment::MAX_GAP_MS);

    if (!valid) {
      result.invalid_working_samples++;
      finish_valid_run();
      have_previous_pressure = false;
      continue;
    }

    result.valid_working_samples++;
    contiguous_valid_samples++;

    // The lower bound must stay well below the nominal period: ticks routinely
    // land a millisecond or two early, and a bound set at the period itself
    // would let those samples skip the check entirely.
    const float allowed_step_bar =
        TrainingAssessment::MAX_STEP_BAR_PER_S * (gap_ms / 1000.0f);
    if (have_previous_pressure &&
        gap_ms >= TrainingAssessment::NOMINAL_PERIOD_MS / 2U &&
        gap_ms <= TrainingAssessment::MAX_GAP_MS &&
        std::fabs(sample.pressure_bar - previous_pressure_bar) >
            allowed_step_bar) {
      result.pressure_jump_events++;
      finish_valid_run();
      contiguous_valid_samples = 1;
    }
    previous_pressure_bar = sample.pressure_bar;
    have_previous_pressure = true;

    // Same scoping as the shot verdict: only the brew phase, and only after
    // the pressure first caught up with the profile. Preinfusion holds a
    // deliberate offset of a couple of bar, and counting it here would make
    // the scoreboard measure the profile shape rather than control quality.
    if (sample.phase_kind ==
            static_cast<uint8_t>(silvia::ShotPhaseKind::BREW) &&
        !tracking_started && sample.target_bar > 0.5f &&
        sample.pressure_bar >= 0.9f * sample.target_bar)
      tracking_started = true;

    if (tracking_started && sample.pi_enabled && sample.sensor_ok &&
        sample.phase_kind ==
            static_cast<uint8_t>(silvia::ShotPhaseKind::BREW) &&
        std::isfinite(sample.pressure_bar) &&
        std::isfinite(sample.target_bar)) {
      const float error = sample.pressure_bar - sample.target_bar;
      tracking_error_sum += error;
      tracking_square_sum += static_cast<double>(error) * error;
      result.tracking_samples++;
      const float magnitude = std::fabs(error);
      if (magnitude > result.tracking_max_abs_error_bar)
        result.tracking_max_abs_error_bar = magnitude;
    }
  }
  finish_valid_run();

  if (result.tracking_samples > 0) {
    const double count = static_cast<double>(result.tracking_samples);
    result.tracking_rmse_bar =
        static_cast<float>(std::sqrt(tracking_square_sum / count));
    result.tracking_mean_error_bar =
        static_cast<float>(tracking_error_sum / count);
  }

  if (result.working_samples == 0)
    result.issues |= TRAINING_ISSUE_NO_PRESSURE_PHASE;
  if (result.valid_working_samples < TrainingAssessment::MIN_VALID_SAMPLES)
    result.issues |= TRAINING_ISSUE_TOO_SHORT;
  if (result.sensor_errors > 0)
    result.issues |= TRAINING_ISSUE_SENSOR_ERROR;
  if (result.invalid_working_samples > 0)
    result.issues |= TRAINING_ISSUE_SENSOR_INVALID;
  if (result.maximum_sensor_age_ms > 300U)
    result.issues |= TRAINING_ISSUE_SENSOR_STALE;
  if (result.maximum_consecutive_errors > 0)
    result.issues |= TRAINING_ISSUE_CONSECUTIVE_ERRORS;
  if (result.timeline_gap_events > 0)
    result.issues |= TRAINING_ISSUE_TIMELINE_GAP;
  if (result.pressure_jump_events > 0)
    result.issues |= TRAINING_ISSUE_PRESSURE_JUMP;
  if (result.usable_windows == 0)
    result.issues |= TRAINING_ISSUE_NO_USABLE_WINDOWS;

  const uint32_t strict_issues =
      TRAINING_ISSUE_NO_PRESSURE_PHASE |
      TRAINING_ISSUE_TOO_SHORT |
      TRAINING_ISSUE_SENSOR_ERROR |
      TRAINING_ISSUE_SENSOR_INVALID |
      TRAINING_ISSUE_SENSOR_STALE |
      TRAINING_ISSUE_SENSOR_FAIL |
      TRAINING_ISSUE_CONSECUTIVE_ERRORS |
      TRAINING_ISSUE_TIMELINE_GAP |
      TRAINING_ISSUE_PRESSURE_JUMP |
      TRAINING_ISSUE_NO_USABLE_WINDOWS;
  result.eligible = (result.issues & strict_issues) == 0;
  if (result.eligible) {
    result.status = "clean";
  } else {
    const float valid_ratio = result.working_samples > 0
                                  ? static_cast<float>(result.valid_working_samples) /
                                        static_cast<float>(result.working_samples)
                                  : 0.0f;
    const bool recoverable =
        (result.issues & TRAINING_ISSUE_SENSOR_FAIL) == 0 &&
        result.valid_working_samples >= 25 &&
        result.usable_windows > 0 &&
        valid_ratio >= 0.90f;
    result.status = recoverable ? "partial" : "rejected";
  }
  return result;
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
  size_t scored_samples = 0;
  bool tracking_started = false;
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
    result.maximum_overshoot_bar =
        std::max(result.maximum_overshoot_bar, error);

    /*
     * Tracking statistics are scored only where the controller is actually
     * supposed to be following the curve.
     *
     * Two regions are deliberately excluded. Preinfusion holds the pump well
     * below the nominal target on purpose, so it contributes a sustained error
     * of a couple of bar that has nothing to do with control quality. And the
     * initial climb towards the profile is a transient: the error there shrinks
     * from "everything" to "nothing" by definition.
     *
     * Averaging over both regions turns a monotonic trend into what looks like
     * a large spread, and the shot then gets reported as oscillating when
     * nothing oscillated at all. Scoring begins once the pressure has caught up
     * to within 10 % of the target for the first time, which adapts itself to
     * the profile instead of relying on a fixed number of seconds.
     */
    const bool brew_phase =
        sample.phase_kind ==
        static_cast<uint8_t>(silvia::ShotPhaseKind::BREW);
    if (brew_phase && !tracking_started && sample.target_bar > 0.5f &&
        sample.pressure_bar >= 0.9f * sample.target_bar)
      tracking_started = true;

    if (brew_phase && tracking_started) {
      absolute_error_sum += std::fabs(error);
      error_sum += error;
      error_square_sum += static_cast<double>(error) * error;
      scored_samples++;
    }

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
  if (scored_samples >= 3) {
    const double count = static_cast<double>(scored_samples);
    result.mean_absolute_error_bar =
        static_cast<float>(absolute_error_sum / count);
    result.mean_error_bar = static_cast<float>(error_sum / count);
    const double mean = error_sum / count;
    const double error_variance =
        std::max(0.0, error_square_sum / count - mean * mean);
    result.pressure_instability_bar =
        static_cast<float>(std::sqrt(error_variance));
  }

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
       (std::isfinite(result.pressure_instability_bar) &&
        result.pressure_instability_bar > 0.85f &&
        std::isfinite(result.flow_variation) && result.flow_variation > 0.45f));

  int score = 100;
  // The tracking statistics stay NAN when a shot never reached its profile, so
  // every term that consumes them has to tolerate that.
  if (std::isfinite(result.mean_absolute_error_bar))
    score -= std::min(30, static_cast<int>(std::lround(
                             std::max(0.0f, result.mean_absolute_error_bar - 0.20f) *
                             18.0f)));
  score -= std::min(22, static_cast<int>(std::lround(
                           std::max(0.0f, result.maximum_overshoot_bar - 0.30f) *
                           12.0f)));
  if (std::isfinite(result.pressure_instability_bar))
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
