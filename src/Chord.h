#pragma once

#include "Rand.h"
#include "Steve.h"

namespace steve {
  class Chord {
  protected:
    static std::vector<ToneSet> _chords;

  public:
    static ToneSet possibilities(const ToneSet& scale, const ToneSet& played);
    static std::vector<ToneSet> in(const ToneSet& scale, const ToneSet& played);
    static bool harmony(const ToneSet& played);
    inline static void add(const std::initializer_list<uint8_t>& il) { _chords.emplace_back(il); }
    inline static ToneSet random() { return Rand::in(_chords); }
  };
}
