#pragma once

#include <memory>
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

    static std::vector<std::shared_ptr<Description>> _descriptions;

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

    // Return chords that fit inside a toneset
    static std::vector<Chord> chords_inside(ToneSet tones);

    static void add(const char* name, const char* suffix, std::initializer_list<uint8_t> tone_list);
  };
}
