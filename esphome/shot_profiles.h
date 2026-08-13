#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

namespace silvia {

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

inline std::vector<ShotPhase> active_shot_phases;
inline std::vector<ShotPhase> custom_shot_phases;
inline bool custom_shot_phases_active = false;

inline bool set_custom_shot_phases(const std::vector<ShotPhase> &phases) {
  if (phases.empty() || phases.size() > 12)
    return false;
  uint64_t total_ms = 0;
  bool has_pump_phase = false;
  float previous_weight = 0.0f;
  for (const auto &phase : phases) {
    if (phase.duration_ms == 0)
      return false;
    total_ms += phase.duration_ms;
    if (total_ms > 240000U)
      return false;
    if (phase.pump_enabled) {
      has_pump_phase = true;
      if (phase.mode != ShotControlMode::PRESSURE ||
          phase.start_value < 0.0f || phase.start_value > 12.0f ||
          phase.end_value < 0.0f || phase.end_value > 12.0f)
        return false;
      if (phase.stop_weight_fraction > 0.0f) {
        if (phase.stop_weight_fraction <= previous_weight ||
            phase.stop_weight_fraction > 1.0f)
          return false;
        previous_weight = phase.stop_weight_fraction;
      }
    }
  }
  if (!has_pump_phase)
    return false;
  custom_shot_phases = phases;
  custom_shot_phases_active = true;
  return true;
}

inline void clear_custom_shot_phases() {
  custom_shot_phases.clear();
  custom_shot_phases_active = false;
}

inline float clamp_percent(float value) {
  return std::max(0.0f, std::min(100.0f, value));
}

inline float smoothstep(float x) {
  x = std::max(0.0f, std::min(1.0f, x));
  return x * x * (3.0f - 2.0f * x);
}

inline uint32_t seconds_to_ms(float seconds) {
  if (seconds <= 0.0f) return 0;
  return static_cast<uint32_t>(seconds * 1000.0f);
}

inline uint32_t bounded_phase_ms(uint32_t preferred_ms, uint32_t available_ms) {
  return std::min(preferred_ms, available_ms);
}

inline void add_power_phase(std::vector<ShotPhase> &phases, ShotPhaseKind kind,
                            uint32_t duration_ms, float start_value,
                            float end_value, bool pump_enabled, bool valve_open,
                            const char *status,
                            float stop_weight_fraction = 0.0f) {
  if (duration_ms == 0) return;
  phases.push_back(ShotPhase{
      ShotControlMode::POWER,
      kind,
      duration_ms,
      clamp_percent(start_value),
      clamp_percent(end_value),
      pump_enabled,
      valve_open,
      std::max(0.0f, std::min(1.0f, stop_weight_fraction)),
      status,
  });
}

inline float clamp_pressure_bar(float value) {
  return std::max(0.0f, std::min(12.0f, value));
}

inline void add_pressure_phase(std::vector<ShotPhase> &phases,
                               ShotPhaseKind kind, uint32_t duration_ms,
                               float start_bar, float end_bar,
                               bool pump_enabled, bool valve_open,
                               const char *status,
                               float stop_weight_fraction = 0.0f) {
  if (duration_ms == 0) return;
  phases.push_back(ShotPhase{
      ShotControlMode::PRESSURE,
      kind,
      duration_ms,
      clamp_pressure_bar(start_bar),
      clamp_pressure_bar(end_bar),
      pump_enabled,
      valve_open,
      std::max(0.0f, std::min(1.0f, stop_weight_fraction)),
      status,
  });
}

inline void add_profile_brew_phases(std::vector<ShotPhase> &phases,
                                    const std::string &profile,
                                    uint32_t brew_ms, float start_pressure,
                                    float main_pressure, float end_pressure) {
  if (brew_ms == 0) return;

  if (profile == "Classic") {
    add_pressure_phase(phases, ShotPhaseKind::BREW, brew_ms, main_pressure,
                       main_pressure, true, true, "Пролив");
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

  add_pressure_phase(phases, ShotPhaseKind::BREW, ramp_ms, start_pressure,
                     main_pressure, true, true, "Пролив");
  add_pressure_phase(phases, ShotPhaseKind::BREW, finish_ms, main_pressure,
                     end_pressure, true, true, "Пролив");
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

  add_pressure_phase(shot.phases, ShotPhaseKind::PREFUSION,
                     seconds_to_ms(preinfusion_seconds), start_pressure,
                     start_pressure, true, true,
                     "Предсмачивание");

  add_power_phase(shot.phases, ShotPhaseKind::SOAK, seconds_to_ms(pause_seconds),
                  0.0f, 0.0f, false, true, "Пауза");

  add_profile_brew_phases(shot.phases, profile, seconds_to_ms(brew_seconds),
                          start_pressure, main_pressure, end_pressure);

  return shot;
}

inline uint32_t phase_steps(const ShotPhase &phase, uint32_t update_ms = 200) {
  if (phase.duration_ms == 0) return 0;
  const uint32_t steps = (phase.duration_ms + update_ms - 1) / update_ms;
  return std::max<uint32_t>(1, steps);
}

inline float phase_value(const ShotPhase &phase, uint32_t step,
                         uint32_t total_steps) {
  if (total_steps <= 1) return phase.end_value / 100.0f;
  const float t = static_cast<float>(step) / static_cast<float>(total_steps - 1);
  const float value = phase.start_value +
                      (phase.end_value - phase.start_value) * smoothstep(t);
  return clamp_percent(value) / 100.0f;
}

inline float phase_target(const ShotPhase &phase, uint32_t step,
                          uint32_t total_steps) {
  if (total_steps <= 1) return phase.end_value;
  const float t = static_cast<float>(step) / static_cast<float>(total_steps - 1);
  return phase.start_value +
         (phase.end_value - phase.start_value) * smoothstep(t);
}

}  // namespace silvia
