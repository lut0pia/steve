#pragma once

#include <cstdint>
#include <algorithm>
#include <set>
#include <vector>

namespace steve {
  class Rand {
  private:
    static uint64_t _state;

  public:
    static uint64_t next();
    static uint8_t next_byte();
    static int next_int();
    static float next_float();

    static uint64_t next(uint64_t min, uint64_t max); // Returns a random unsigned integer between min and max
    static int next(int min, int max); // Returns a random integer between min and max
    static float next(float min, float max); // Returns a random float between min and max

    static float gauss(uint32_t);
    static uint64_t next(uint64_t ave); // Random exp with an average n

    // Containers
    template <class T>
    static const T& in(const std::set<T>& s) {
      auto it(s.begin());
      std::advance(it, next(0, s.size()-1));
      return *it;
    }
    template <class T>
    inline static const T& in(const std::vector<T>& v) {
      return v[next(0, v.size()-1)];
    }

    inline static const uint64_t& state() { return _state; }
    inline static void state(const uint64_t& s) { _state = s; }
  };
}
