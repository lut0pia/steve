#pragma once

#include <string>
#include "Rand.h"
#include "Steve.h"

namespace steve {
  class Chord {
  protected:
    struct Description {
      Description(const char* name, const char* suffix, std::initializer_list<uint8_t> tone_list);
      std::string name, suffix;
      ToneSet tones;
    };

    static std::vector<Description> _descriptions;

    const Description& _desc;
    uint8_t _key;
    ToneSet _tones;

    inline Chord(const Description& desc, uint8_t key) :
      _desc(desc), _key(key), _tones(tone_set_shift(desc.tones, key)) {
    }

  public:
    std::string to_short_string() const;
    inline uint8_t key() const { return _key; }
    inline ToneSet tones() const { return _tones; }

    // Return chords that fit inside the scale
    static std::vector<Chord> chords_inside(const class Scale& scale);

    static void add(const char* name, const char* suffix, std::initializer_list<uint8_t> tone_list);
  };
}
