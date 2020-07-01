#include "Rand.h"

#include <random>

static std::default_random_engine generator;

using namespace steve;

float Rand::next_float() {
  std::uniform_real_distribution<float> dist(0.f, 1.f);
  return dist(generator);
}

float Rand::next_normal() {
  std::normal_distribution<float> dist(0.5f, 0.1f);
  return std::max(0.f, std::min(1.f, dist(generator)));
}

uint64_t Rand::next(uint64_t min, uint64_t max) {
  std::uniform_int_distribution<uint64_t> dist(min, max);
  return dist(generator);
}
int Rand::next(int min, int max) {
  std::uniform_int_distribution<int> dist(min, max);
  return dist(generator);
}
float Rand::next(float min, float max) {
  std::uniform_real_distribution<float> dist(min, max);
  return dist(generator);
}
