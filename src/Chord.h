#pragma once

#include <memory>
#include <string>

#include "Steve.h"

namespace steve {
  struct ChordDescription {
    std::string name, suffix;
    ToneSet tones = 1;
  };

  struct Chord {
  public:
    std::shared_ptr<const ChordDescription> desc;
    uint8_t key;
    ToneSet tones;

    inline Chord(std::shared_ptr<const ChordDescription> desc, uint8_t key)
      : desc(desc), key(key), tones(tone_set_shift(desc->tones, key)) {
    }

    std::string to_short_string() const;
    inline Chord shifted(uint8_t semitones) const { return Chord(desc, (key + semitones) % 12); }
  };
}
