#pragma once

#include <memory>
#include <string>
#include "Rand.h"
#include "Steve.h"

namespace steve {
  class Chord {
  public:
    struct Description {
      std::string name, suffix;
      ToneSet tones = 1;
    };

  protected:
    std::shared_ptr<const Description> _desc;
    uint8_t _key;
    ToneSet _tones;

    inline Chord(std::shared_ptr<const Description> desc, uint8_t key) :
      _desc(desc), _key(key), _tones(tone_set_shift(desc->tones, key)) {
    }

  public:
    std::string to_short_string() const;
    inline uint8_t key() const { return _key; }
    inline ToneSet tones() const { return _tones; }
    inline Chord shifted(uint8_t semitones) const { return Chord(_desc, (_key + semitones) % 12); }

    friend class Config;
  };
}
