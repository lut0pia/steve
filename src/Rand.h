#pragma once

#include <cstdint>
#include <iterator>
#include <set>
#include <vector>

namespace steve {
  class Rand {
  public:
    static float next_float();
    static float next_normal();

    static uint64_t next(uint64_t min, uint64_t max); // Returns a random unsigned integer between min and max
    static int next(int min, int max); // Returns a random integer between min and max
    static float next(float min, float max); // Returns a random float between min and max

    // Containers
    template <class T>
    static const T& in(const std::set<T>& s) {
      auto it(s.begin());
      std::advance(it, next(0ull, s.size()-1));
      return *it;
    }
    template <class T>
    inline static const T& in(const std::vector<T>& v) {
      return v[(unsigned int)next(0ull, v.size()-1)];
    }
  };
}
