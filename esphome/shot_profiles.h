#pragma once

#include <algorithm>
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
  bool start_boost;
  const char *status;
};

struct ShotProfile {
  std::string name;
  std::vector<ShotPhase> phases;
};

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
                            bool start_boost, const char *status) {
  if (duration_ms == 0) return;
  phases.push_back(ShotPhase{
      ShotControlMode::POWER,
      kind,
      duration_ms,
      clamp_percent(start_value),
      clamp_percent(end_value),
      pump_enabled,
      valve_open,
      start_boost,
      status,
  });
}

inline void add_profile_brew_phases(std::vector<ShotPhase> &phases,
                                    const std::string &profile,
                                    uint32_t brew_ms, float start_power,
                                    float main_power, float end_power,
                                    bool main_boost) {
  if (brew_ms == 0) return;

  if (profile == "Classic") {
    add_power_phase(phases, ShotPhaseKind::BREW, brew_ms, 100.0f, 100.0f, true,
                    true, main_boost, "Пролив");
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

  add_power_phase(phases, ShotPhaseKind::BREW, ramp_ms, start_power, main_power,
                  true, true, main_boost, "Пролив");
  add_power_phase(phases, ShotPhaseKind::BREW, finish_ms, main_power, end_power,
                  true, true, false, "Пролив");
}

inline ShotProfile build_shot_profile(const std::string &profile,
                                      float preinfusion_seconds,
                                      float pause_seconds,
                                      float brew_seconds,
                                      float start_power,
                                      float main_power,
                                      float end_power,
                                      bool preinfusion_boost,
                                      bool main_boost) {
  ShotProfile shot;
  shot.name = profile;

  add_power_phase(shot.phases, ShotPhaseKind::PREFUSION,
                  seconds_to_ms(preinfusion_seconds), start_power, start_power,
                  true, true, preinfusion_boost, "Предсмачивание");

  add_power_phase(shot.phases, ShotPhaseKind::SOAK, seconds_to_ms(pause_seconds),
                  0.0f, 0.0f, false, true, false, "Пауза");

  add_profile_brew_phases(shot.phases, profile, seconds_to_ms(brew_seconds),
                          start_power, main_power, end_power, main_boost);

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

}  // namespace silvia
