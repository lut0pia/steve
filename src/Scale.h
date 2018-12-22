#pragma once

#include <vector>
#include <initializer_list>
#include "Rand.h"
#include "Steve.h"

namespace steve {
  class Scale {
  protected:
    static std::vector<Scale> scales;

    std::string _name;
    int _key;
    ToneSet _tones;

    inline Scale(const Scale& scale, int key) :
      _name(scale._name), _key(key), _tones(shift(scale._tones, -key)) {
    }

  public:
    Scale(const char* name, std::initializer_list<uint8_t> tones);
    inline std::string name() const { return std::string(key_name(_key)) + " " + _name; }
    inline ToneSet tones() const { return _tones; }
    static inline void add(const Scale& scale) { scales.push_back(scale); }
    static inline Scale random() { return Scale(scales[Rand::next(0u, scales.size()-1)], Rand::next(0, 11)); }
  };
}
