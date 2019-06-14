#pragma once

#include "Rand.h"
#include "Steve.h"

namespace steve {
  class Chord {
  protected:
    static std::vector<Chord> _chords;

    std::string _name, _suffix;
    int _key;
    ToneSet _tones;

    inline Chord(const Chord& chord, int key) :
      _name(chord._name), _suffix(chord._suffix), _key(key), _tones(tone_set_shift(chord._tones, key)) {}

  public:
    Chord(const char* name, const char* suffix, std::initializer_list<uint8_t> tones);
    std::string to_short_string() const;
    inline ToneSet tones() const { return _tones; }

    // Return chords that fit inside the scale
    static std::vector<Chord> chords_inside(const class Scale& scale);

    static void add(const Chord& chord);
  };
}
