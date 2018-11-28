#pragma once

#include <set>
#include <vector>
#include <initializer_list>
#include "Rand.h"
#include "Steve.h"

namespace steve {
  class Scale : public std::set<uint8_t> {
  protected:
    static std::vector<Scale> scales;

    int _key;
    std::string _name;

    inline Scale(const Scale& scale, int key) :
      std::set<uint8_t>(shift(scale, -key)),
      _key(key), _name(scale._name) {
    }

  public:
    inline Scale(const char* name, std::initializer_list<uint8_t> tones) :
      std::set<uint8_t>(tones),
      _key(0), _name(name) {
    }
    inline std::string name() const { return std::string(key_name(_key)) + " " + _name; }
    static inline void add(const Scale& scale) { scales.push_back(scale); }
    static inline Scale Scale::random() { return Scale(scales[Rand::next(0, scales.size()-1)], Rand::next(0, 11)); }
  };
}
