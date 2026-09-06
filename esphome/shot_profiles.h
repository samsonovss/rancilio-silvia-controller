#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace silvia {

// -----------------------------------------------------------------------------
// Shot phase model
// -----------------------------------------------------------------------------
//
// The firmware still keeps the original PREFUSION / SOAK / BREW kinds because
// rancilio-silvia-power.yaml uses them for shot status and diagnostics.
//
// For the new universal editor the user-facing model is simpler:
//   - pump_enabled == true  -> pressure/delivery phase
//   - pump_enabled == false -> pause phase
//
// This keeps the existing shot runner fully compatible while allowing an
// arbitrary sequence such as:
//   delivery -> pause -> delivery -> pause -> delivery ...
// -----------------------------------------------------------------------------

enum class ShotControlMode {
  POWER,
  PRESSURE,
  FLOW,
};

enum class ShotPhaseKind {
  PREFUSION,
  SOAK,
  BREW,
};

enum class EditableShotPhaseType {
  DELIVERY,
  PAUSE,
};

// PREFUSION remains a semantic role of a pressure-delivery phase. The visual
// editor must preserve it because startup control, status and diagnostics use
// it differently from an ordinary BREW delivery.

struct ShotPhase {
  ShotControlMode mode;
  ShotPhaseKind kind;
  uint32_t duration_ms;
  float start_value;
  float end_value;
  bool pump_enabled;
  bool valve_open;
  float stop_weight_fraction;
  const char *status;
};

struct ShotProfile {
  std::string name;
  std::vector<ShotPhase> phases;
};

// Limits used by the web editor and by custom profile validation.
inline constexpr std::size_t MAX_CUSTOM_SHOT_PHASES = 12;
inline constexpr uint32_t MAX_CUSTOM_SHOT_DURATION_MS = 240000U;
inline constexpr uint32_t DEFAULT_DELIVERY_DURATION_MS = 20000U;
inline constexpr uint32_t DEFAULT_PAUSE_DURATION_MS = 10000U;
inline constexpr float DEFAULT_DELIVERY_PRESSURE_BAR = 0.8f;

// The currently executing shot and the currently loaded editable/custom shot.
inline std::vector<ShotPhase> active_shot_phases;
inline std::vector<ShotPhase> custom_shot_phases;
inline bool custom_shot_phases_active = false;

// -----------------------------------------------------------------------------
// Small helpers
// -----------------------------------------------------------------------------

inline float clamp_percent(float value) {
  return std::max(0.0f, std::min(100.0f, value));
}

inline float clamp_pressure_bar(float value) {
  return std::max(0.0f, std::min(12.0f, value));
}

inline float clamp_weight_fraction(float value) {
  return std::max(0.0f, std::min(1.0f, value));
}

inline float smoothstep(float x) {
  x = std::max(0.0f, std::min(1.0f, x));
  return x * x * (3.0f - 2.0f * x);
}

inline uint32_t seconds_to_ms(float seconds) {
  if (seconds <= 0.0f)
    return 0;
  return static_cast<uint32_t>(seconds * 1000.0f);
}

inline float ms_to_seconds(uint32_t milliseconds) {
  return static_cast<float>(milliseconds) / 1000.0f;
}

inline uint32_t bounded_phase_ms(uint32_t preferred_ms,
                                 uint32_t available_ms) {
  return std::min(preferred_ms, available_ms);
}

inline EditableShotPhaseType editable_phase_type(const ShotPhase &phase) {
  return phase.pump_enabled ? EditableShotPhaseType::DELIVERY
                            : EditableShotPhaseType::PAUSE;
}

inline bool is_delivery_phase(const ShotPhase &phase) {
  return phase.pump_enabled;
}

inline bool is_pause_phase(const ShotPhase &phase) {
  return !phase.pump_enabled;
}

inline const char *default_phase_status(const ShotPhase &phase) {
  if (!phase.pump_enabled)
    return "Пауза";
  if (phase.kind == ShotPhaseKind::PREFUSION)
    return "Предсмачивание";
  return "Подача";
}

inline uint32_t shot_phases_total_duration_ms(
    const std::vector<ShotPhase> &phases) {
  uint64_t total = 0;
  for (const auto &phase : phases)
    total += phase.duration_ms;

  if (total > UINT32_MAX)
    return UINT32_MAX;
  return static_cast<uint32_t>(total);
}

inline std::size_t shot_phases_delivery_count(
    const std::vector<ShotPhase> &phases) {
  std::size_t count = 0;
  for (const auto &phase : phases) {
    if (phase.pump_enabled)
      ++count;
  }
  return count;
}

inline std::size_t shot_phases_pause_count(
    const std::vector<ShotPhase> &phases) {
  std::size_t count = 0;
  for (const auto &phase : phases) {
    if (!phase.pump_enabled)
      ++count;
  }
  return count;
}

// -----------------------------------------------------------------------------
// Phase constructors
// -----------------------------------------------------------------------------
//
// Existing add_power_phase()/add_pressure_phase() are intentionally preserved
// because the current built-in profiles already use them.
//
// make_delivery_phase()/make_pause_phase() are the preferred helpers for the
// universal editor and user profiles.
// -----------------------------------------------------------------------------

inline ShotPhase make_delivery_phase(
    uint32_t duration_ms = DEFAULT_DELIVERY_DURATION_MS,
    float start_bar = DEFAULT_DELIVERY_PRESSURE_BAR,
    float end_bar = DEFAULT_DELIVERY_PRESSURE_BAR,
    float stop_weight_fraction = 0.0f,
    ShotPhaseKind kind = ShotPhaseKind::BREW,
    const char *status = "Подача") {
  return ShotPhase{
      ShotControlMode::PRESSURE,
      kind,
      duration_ms,
      clamp_pressure_bar(start_bar),
      clamp_pressure_bar(end_bar),
      true,
      true,
      clamp_weight_fraction(stop_weight_fraction),
      status,
  };
}

inline ShotPhase make_pause_phase(
    uint32_t duration_ms = DEFAULT_PAUSE_DURATION_MS,
    const char *status = "Пауза") {
  return ShotPhase{
      ShotControlMode::POWER,
      ShotPhaseKind::SOAK,
      duration_ms,
      0.0f,
      0.0f,
      false,
      true,
      0.0f,
      status,
  };
}

inline void add_power_phase(std::vector<ShotPhase> &phases,
                            ShotPhaseKind kind,
                            uint32_t duration_ms,
                            float start_value,
                            float end_value,
                            bool pump_enabled,
                            bool valve_open,
                            const char *status,
                            float stop_weight_fraction = 0.0f) {
  if (duration_ms == 0)
    return;

  phases.push_back(ShotPhase{
      ShotControlMode::POWER,
      kind,
      duration_ms,
      clamp_percent(start_value),
      clamp_percent(end_value),
      pump_enabled,
      valve_open,
      clamp_weight_fraction(stop_weight_fraction),
      status,
  });
}

inline void add_pressure_phase(std::vector<ShotPhase> &phases,
                               ShotPhaseKind kind,
                               uint32_t duration_ms,
                               float start_bar,
                               float end_bar,
                               bool pump_enabled,
                               bool valve_open,
                               const char *status,
                               float stop_weight_fraction = 0.0f) {
  if (duration_ms == 0)
    return;

  phases.push_back(ShotPhase{
      ShotControlMode::PRESSURE,
      kind,
      duration_ms,
      clamp_pressure_bar(start_bar),
      clamp_pressure_bar(end_bar),
      pump_enabled,
      valve_open,
      clamp_weight_fraction(stop_weight_fraction),
      status,
  });
}

inline bool append_delivery_phase(
    std::vector<ShotPhase> &phases,
    uint32_t duration_ms = DEFAULT_DELIVERY_DURATION_MS,
    float start_bar = DEFAULT_DELIVERY_PRESSURE_BAR,
    float end_bar = DEFAULT_DELIVERY_PRESSURE_BAR,
    float stop_weight_fraction = 0.0f) {
  if (phases.size() >= MAX_CUSTOM_SHOT_PHASES)
    return false;

  phases.push_back(make_delivery_phase(duration_ms, start_bar, end_bar,
                                       stop_weight_fraction));
  return true;
}

inline bool append_pause_phase(
    std::vector<ShotPhase> &phases,
    uint32_t duration_ms = DEFAULT_PAUSE_DURATION_MS) {
  if (phases.size() >= MAX_CUSTOM_SHOT_PHASES)
    return false;

  phases.push_back(make_pause_phase(duration_ms));
  return true;
}

// -----------------------------------------------------------------------------
// Universal editor operations
// -----------------------------------------------------------------------------

inline bool insert_shot_phase(std::vector<ShotPhase> &phases,
                              std::size_t index,
                              const ShotPhase &phase) {
  if (phases.size() >= MAX_CUSTOM_SHOT_PHASES)
    return false;

  if (index > phases.size())
    index = phases.size();

  phases.insert(phases.begin() + index, phase);
  return true;
}

inline bool erase_shot_phase(std::vector<ShotPhase> &phases,
                             std::size_t index) {
  if (index >= phases.size())
    return false;

  phases.erase(phases.begin() + index);
  return true;
}

inline bool duplicate_shot_phase(std::vector<ShotPhase> &phases,
                                 std::size_t index) {
  if (index >= phases.size() || phases.size() >= MAX_CUSTOM_SHOT_PHASES)
    return false;

  phases.insert(phases.begin() + index + 1, phases[index]);
  return true;
}

inline bool move_shot_phase(std::vector<ShotPhase> &phases,
                            std::size_t from,
                            std::size_t to) {
  if (from >= phases.size() || to >= phases.size())
    return false;

  if (from == to)
    return true;

  ShotPhase phase = phases[from];
  phases.erase(phases.begin() + from);
  phases.insert(phases.begin() + to, phase);
  return true;
}

inline bool set_shot_phase_duration(std::vector<ShotPhase> &phases,
                                    std::size_t index,
                                    uint32_t duration_ms) {
  if (index >= phases.size() || duration_ms == 0)
    return false;

  uint64_t total_without_phase = 0;
  for (std::size_t i = 0; i < phases.size(); ++i) {
    if (i != index)
      total_without_phase += phases[i].duration_ms;
  }

  if (total_without_phase + duration_ms > MAX_CUSTOM_SHOT_DURATION_MS)
    return false;

  phases[index].duration_ms = duration_ms;
  return true;
}

inline bool set_delivery_phase_pressure(std::vector<ShotPhase> &phases,
                                        std::size_t index,
                                        float start_bar,
                                        float end_bar) {
  if (index >= phases.size() || !phases[index].pump_enabled)
    return false;

  if (!std::isfinite(start_bar) || !std::isfinite(end_bar) ||
      start_bar < 0.0f || start_bar > 12.0f ||
      end_bar < 0.0f || end_bar > 12.0f)
    return false;

  phases[index].mode = ShotControlMode::PRESSURE;
  phases[index].start_value = start_bar;
  phases[index].end_value = end_bar;
  return true;
}

inline bool set_delivery_phase_stop_weight(std::vector<ShotPhase> &phases,
                                           std::size_t index,
                                           float fraction) {
  if (index >= phases.size() || !phases[index].pump_enabled)
    return false;

  if (!std::isfinite(fraction) || fraction < 0.0f || fraction > 1.0f)
    return false;

  phases[index].stop_weight_fraction = fraction;
  return true;
}

// Converts a phase between the two user-facing phase types while preserving its
// duration. Pressure defaults are deliberately conservative for a new delivery.
inline bool convert_shot_phase_type(std::vector<ShotPhase> &phases,
                                    std::size_t index,
                                    EditableShotPhaseType type) {
  if (index >= phases.size())
    return false;

  const uint32_t duration_ms = phases[index].duration_ms;

  if (type == EditableShotPhaseType::PAUSE) {
    phases[index] = make_pause_phase(duration_ms);
  } else {
    phases[index] =
        make_delivery_phase(duration_ms,
                            DEFAULT_DELIVERY_PRESSURE_BAR,
                            DEFAULT_DELIVERY_PRESSURE_BAR);
  }

  return true;
}

// -----------------------------------------------------------------------------
// Custom profile validation / activation
// -----------------------------------------------------------------------------

inline bool validate_custom_shot_phases(
    const std::vector<ShotPhase> &phases,
    std::string *error = nullptr) {
  if (error != nullptr)
    error->clear();

  auto fail = [error](const char *message) {
    if (error != nullptr)
      *error = message;
    return false;
  };

  if (phases.empty())
    return fail("profile has no phases");

  if (phases.size() > MAX_CUSTOM_SHOT_PHASES)
    return fail("too many phases");

  uint64_t total_ms = 0;
  bool has_delivery = false;
  float previous_stop_weight = 0.0f;

  for (std::size_t i = 0; i < phases.size(); ++i) {
    const auto &phase = phases[i];

    if (phase.duration_ms == 0)
      return fail("phase duration is zero");

    total_ms += phase.duration_ms;
    if (total_ms > MAX_CUSTOM_SHOT_DURATION_MS)
      return fail("profile duration is too long");

    if (!std::isfinite(phase.start_value) ||
        !std::isfinite(phase.end_value) ||
        !std::isfinite(phase.stop_weight_fraction))
      return fail("phase contains a non-finite value");

    if (!phase.pump_enabled)
      continue;

    has_delivery = true;

    // The universal editor currently creates pressure-controlled delivery
    // phases. FLOW remains reserved for future use.
    if (phase.mode != ShotControlMode::PRESSURE)
      return fail("delivery phase must use pressure control");

    if (phase.start_value < 0.0f || phase.start_value > 12.0f ||
        phase.end_value < 0.0f || phase.end_value > 12.0f)
      return fail("pressure is outside 0..12 bar");

    const float stop_weight = phase.stop_weight_fraction;
    if (stop_weight < 0.0f || stop_weight > 1.0f)
      return fail("stop weight is outside 0..1");

    // Zero means "do not stop this phase by weight".
    if (stop_weight > 0.0f) {
      if (stop_weight <= previous_stop_weight)
        return fail("stop weights must increase from phase to phase");
      previous_stop_weight = stop_weight;
    }
  }

  if (!has_delivery)
    return fail("profile has no delivery phase");

  return true;
}

// Ensure custom phases have safe static status text for the shot runner.
// Pause phases are normalized to pump-off POWER/SOAK phases. This mirrors the
// current Funnel implementation and prevents a pause from accidentally
// energising the pump.
inline void normalize_custom_shot_phases(
    std::vector<ShotPhase> &phases) {
  for (auto &phase : phases) {
    if (phase.status == nullptr || phase.status[0] == '\0')
      phase.status = default_phase_status(phase);

    if (!phase.pump_enabled) {
      phase.mode = ShotControlMode::POWER;
      phase.kind = ShotPhaseKind::SOAK;
      phase.start_value = 0.0f;
      phase.end_value = 0.0f;
      phase.stop_weight_fraction = 0.0f;
    }
  }
}

inline bool set_custom_shot_phases(const std::vector<ShotPhase> &phases) {
  std::vector<ShotPhase> normalized = phases;
  normalize_custom_shot_phases(normalized);

  if (!validate_custom_shot_phases(normalized))
    return false;

  custom_shot_phases = normalized;
  custom_shot_phases_active = true;
  return true;
}

inline bool set_custom_shot_phases(const std::vector<ShotPhase> &phases,
                                   std::string *error) {
  std::vector<ShotPhase> normalized = phases;
  normalize_custom_shot_phases(normalized);

  if (!validate_custom_shot_phases(normalized, error))
    return false;

  custom_shot_phases = normalized;
  custom_shot_phases_active = true;
  return true;
}

inline void clear_custom_shot_phases() {
  custom_shot_phases.clear();
  custom_shot_phases_active = false;
}

inline ShotProfile make_custom_shot_profile(
    const std::string &name,
    const std::vector<ShotPhase> &phases) {
  ShotProfile shot;
  shot.name = name;
  shot.phases = phases;
  return shot;
}

// -----------------------------------------------------------------------------
// Existing built-in profile builder
// -----------------------------------------------------------------------------
//
// Kept compatible with the current YAML. Later the dashboard can copy any
// built-in profile into custom_shot_phases and edit it as an arbitrary list of
// delivery/pause phases without changing the shot execution engine.
// -----------------------------------------------------------------------------

inline void add_profile_brew_phases(std::vector<ShotPhase> &phases,
                                    const std::string &profile,
                                    uint32_t brew_ms,
                                    float start_pressure,
                                    float main_pressure,
                                    float end_pressure) {
  if (brew_ms == 0)
    return;

  if (profile == "Classic") {
    add_pressure_phase(phases, ShotPhaseKind::BREW, brew_ms,
                       main_pressure, main_pressure,
                       true, true, "Пролив");
    return;
  }

  uint32_t ramp_ms = 3000;
  if (profile == "Slayer Style") {
    ramp_ms = 5000;
  } else if (profile == "Bloom") {
    ramp_ms = 4000;
  }

  ramp_ms = bounded_phase_ms(ramp_ms, brew_ms);
  const uint32_t finish_ms = brew_ms - ramp_ms;

  add_pressure_phase(phases, ShotPhaseKind::BREW, ramp_ms,
                     start_pressure, main_pressure,
                     true, true, "Пролив");

  add_pressure_phase(phases, ShotPhaseKind::BREW, finish_ms,
                     main_pressure, end_pressure,
                     true, true, "Пролив");
}

inline ShotProfile build_shot_profile(const std::string &profile,
                                      float preinfusion_seconds,
                                      float pause_seconds,
                                      float brew_seconds,
                                      float start_pressure,
                                      float main_pressure,
                                      float end_pressure) {
  ShotProfile shot;
  shot.name = profile;

  // Custom profile supplied by the new universal phase editor.
  if (profile == "Custom" && custom_shot_phases_active) {
    shot.phases = custom_shot_phases;
    return shot;
  }

  // Automated pour-over for 15 g of coffee and about 220 g in the cup.
  // Each pump portion stops by cup weight, while fixed phase durations keep
  // the recipe close to three minutes. The hard limit is 3:20.
  if (profile == "Funnel") {
    add_pressure_phase(shot.phases, ShotPhaseKind::PREFUSION, 12000U,
                       0.5f, 0.5f, true, true, "Смачивание");

    add_power_phase(shot.phases, ShotPhaseKind::SOAK, 28000U,
                    0.0f, 0.0f, false, true, "Набухание");

    add_pressure_phase(shot.phases, ShotPhaseKind::BREW, 30000U,
                       0.8f, 0.8f, true, true, "Пролив 1", 0.35f);

    add_power_phase(shot.phases, ShotPhaseKind::SOAK, 15000U,
                    0.0f, 0.0f, false, true, "Пауза 1");

    add_pressure_phase(shot.phases, ShotPhaseKind::BREW, 30000U,
                       0.8f, 0.8f, true, true, "Пролив 2", 0.68f);

    add_power_phase(shot.phases, ShotPhaseKind::SOAK, 15000U,
                    0.0f, 0.0f, false, true, "Пауза 2");

    add_pressure_phase(shot.phases, ShotPhaseKind::BREW, 30000U,
                       0.8f, 0.8f, true, true, "Пролив 3", 0.92f);

    add_power_phase(shot.phases, ShotPhaseKind::SOAK, 40000U,
                    0.0f, 0.0f, false, true, "Стекание");

    return shot;
  }

  // Legacy built-in profile construction. It remains here until the dashboard
  // migration is complete, so replacing only this header does not break the
  // current controls or the current YAML.
  add_pressure_phase(
      shot.phases,
      ShotPhaseKind::PREFUSION,
      seconds_to_ms(preinfusion_seconds),
      start_pressure,
      start_pressure,
      true,
      true,
      "Предсмачивание");

  add_power_phase(
      shot.phases,
      ShotPhaseKind::SOAK,
      seconds_to_ms(pause_seconds),
      0.0f,
      0.0f,
      false,
      true,
      "Пауза");

  add_profile_brew_phases(
      shot.phases,
      profile,
      seconds_to_ms(brew_seconds),
      start_pressure,
      main_pressure,
      end_pressure);

  return shot;
}

// -----------------------------------------------------------------------------
// Shot runner interpolation helpers
// -----------------------------------------------------------------------------

// Period of one shot-runner iteration, in milliseconds.
//
// This MUST match the `delay:` at the end of the run_active_shot_phase loop in
// eso32-s3.yaml. A phase is executed as a fixed number of iterations, so the
// two together decide how long a phase actually lasts in wall-clock time. When
// the loop period was halved to 100 ms and this constant still said 200, every
// phase finished in half its configured time: a 4 s preinfusion ran 2 s and a
// 29 s shot ended after 14.5 s.
inline constexpr uint32_t SHOT_STEP_MS = 100U;

inline uint32_t phase_steps(const ShotPhase &phase,
                            uint32_t update_ms = SHOT_STEP_MS) {
  if (phase.duration_ms == 0)
    return 0;

  const uint32_t steps =
      (phase.duration_ms + update_ms - 1) / update_ms;

  return std::max<uint32_t>(1, steps);
}

inline float phase_value(const ShotPhase &phase,
                         uint32_t step,
                         uint32_t total_steps) {
  if (total_steps <= 1)
    return phase.end_value / 100.0f;

  const float t =
      static_cast<float>(step) /
      static_cast<float>(total_steps - 1);

  const float value =
      phase.start_value +
      (phase.end_value - phase.start_value) * smoothstep(t);

  return clamp_percent(value) / 100.0f;
}

inline float phase_target(const ShotPhase &phase,
                          uint32_t step,
                          uint32_t total_steps) {
  if (total_steps <= 1)
    return phase.end_value;

  const float t =
      static_cast<float>(step) /
      static_cast<float>(total_steps - 1);

  return phase.start_value +
         (phase.end_value - phase.start_value) * smoothstep(t);
}

inline float phase_target_at_elapsed(const ShotPhase &phase,
                                     uint32_t elapsed_ms) {
  if (!phase.pump_enabled || phase.mode != ShotControlMode::PRESSURE)
    return 0.0f;
  if (phase.duration_ms == 0U)
    return phase.end_value;

  const float progress =
      static_cast<float>(std::min(elapsed_ms, phase.duration_ms)) /
      static_cast<float>(phase.duration_ms);
  return phase.start_value +
         (phase.end_value - phase.start_value) * smoothstep(progress);
}

inline float trajectory_target_from(
    const std::vector<ShotPhase> &phases,
    std::size_t phase_index,
    uint32_t phase_elapsed_ms,
    uint32_t lookahead_ms) {
  if (phases.empty() || phase_index >= phases.size())
    return 0.0f;

  uint64_t offset_ms =
      static_cast<uint64_t>(phase_elapsed_ms) + lookahead_ms;
  std::size_t index = phase_index;

  while (index < phases.size()) {
    const ShotPhase &candidate = phases[index];
    if (offset_ms < candidate.duration_ms || index + 1 >= phases.size()) {
      return phase_target_at_elapsed(
          candidate,
          static_cast<uint32_t>(
              std::min<uint64_t>(offset_ms, candidate.duration_ms)));
    }
    offset_ms -= candidate.duration_ms;
    ++index;
  }
  return 0.0f;
}

}  // namespace silvia
