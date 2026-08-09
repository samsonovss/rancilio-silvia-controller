#include <cassert>
#include <cmath>
#include <cstdint>
#include <vector>

#include "../esphome/shot_analyzer.h"

enum class StartupState { WAIT_DROP, PI, SENSOR_FAIL };

struct Sample {
  uint32_t elapsed_ms = 0;
  float target_bar = 9.0f;
  float pressure_bar = 9.0f;
  float flow_g_s = 1.5f;
  float weight_g = 0.0f;
  bool sensor_ok = true;
  uint32_t sensor_age_ms = 20;
  uint32_t xdb_total_errors = 0;
  StartupState startup_state = StartupState::PI;
};

static std::vector<Sample> stable_shot(float duration_s, float final_weight) {
  std::vector<Sample> samples;
  for (int index = 0; index <= 100; index++) {
    Sample sample;
    sample.elapsed_ms = static_cast<uint32_t>(duration_s * 10.0f * index);
    sample.pressure_bar = 9.0f + std::sin(index * 0.2f) * 0.08f;
    sample.weight_g = final_weight * index / 100.0f;
    samples.push_back(sample);
  }
  return samples;
}

int main() {
  const silvia_analysis::ShotMetadata metadata{18.0f, 36.0f, 28.0f};

  const auto good = silvia_analysis::analyze(stable_shot(27.0f, 36.0f), metadata);
  assert(good.reliable);
  assert(good.score >= 85);
  assert(good.diagnosis == "normal");
  assert(good.grind == "none");

  const auto fast = silvia_analysis::analyze(stable_shot(16.0f, 36.0f), metadata);
  assert(fast.diagnosis == "too_free");
  assert(fast.grind == "finer");

  const auto slow = silvia_analysis::analyze(stable_shot(28.0f, 24.0f), metadata);
  assert(slow.diagnosis == "too_restrictive");
  assert(slow.grind == "coarser");

  auto faulty_samples = stable_shot(27.0f, 36.0f);
  for (size_t index = 10; index < faulty_samples.size(); index++) {
    faulty_samples[index].sensor_ok = false;
    faulty_samples[index].sensor_age_ms = 1400;
    faulty_samples[index].xdb_total_errors = static_cast<uint32_t>(index - 9);
  }
  const auto faulty = silvia_analysis::analyze(faulty_samples, metadata);
  assert(!faulty.reliable);
  assert(faulty.diagnosis == "sensor_fault");
  assert(faulty.grind == "unknown");
}
