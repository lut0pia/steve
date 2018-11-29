#pragma once

#include "Rand.h"
#include "Steve.h"

namespace steve {
  class Chord {
  protected:
    static std::vector<ToneSet> _chords;

  public:
    // Return possible tones in harmony with currently played tones, considering scale
    // Don't forget if a single tone is added to the played tones, the result is no longer valid
    static ToneSet tones_in_harmony(const ToneSet& scale, const ToneSet& played);

    // Return possible chords in harmony with currently played tones, considering scale
    static std::vector<ToneSet> chords_in_harmony(const ToneSet& scale, const ToneSet& played);

    // Return true if all tones happen to be in one of the registered chords
    static bool harmony(const ToneSet& played);

    static void add(const std::initializer_list<uint8_t>& tones);
    inline static ToneSet random() { return Rand::in(_chords); }
  };
}
