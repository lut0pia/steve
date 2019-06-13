#pragma once

#include "Rand.h"
#include "Steve.h"

namespace steve {
  class Chord {
  protected:
    static std::vector<ToneSet> _chords;

  public:
    // Return chords that fit inside the scale
    static std::vector<ToneSet> chords_inside(const ToneSet& scale);

    static void add(const std::initializer_list<uint8_t>& tones);
    static const std::vector<ToneSet>& list() { return _chords; }
    inline static ToneSet random() { return Rand::in(_chords); }
  };
}
