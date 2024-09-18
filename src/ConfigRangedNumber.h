#pragma once

#include <cstdint>

#include "Rand.h"

namespace steve {
  struct ConfigRangedNumber {
    float min = 0.f;
    float max = 1.f;
    float step = 0.01f;
    float sigma = 1.f;

    inline float get_value() const {
      std::normal_distribution<float> distribution(0.5f, sigma);
      const float value_range = max - min;
      const float factor = std::max(0.f, std::min(1.f, distribution(Rand::generator)));
      const float unstepped_value = value_range * factor + min;
      return floorf(unstepped_value / step) * step;
    }
  };
}
