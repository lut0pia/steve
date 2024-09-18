#include "ConfigRangedNumber.h"

#include "../Rand.h"

using namespace steve;

float ConfigRangedNumber::get_value() const {
  std::normal_distribution<float> distribution(0.5f, sigma);
  const float value_range = max - min;
  const float factor = std::max(0.f, std::min(1.f, distribution(Rand::generator)));
  const float unstepped_value = value_range * factor + min;
  return floorf(unstepped_value / step) * step;
}
