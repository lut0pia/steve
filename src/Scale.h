#pragma once

#include <string>
#include <vector>
#include <initializer_list>
#include "Chord.h"
#include "Rand.h"
#include "Steve.h"

namespace steve {
  class Scale {
  public:
    struct ChordDescription {
      Chord chord;
      float tonicity = 0;
    };
    struct Description {
      std::string name;
      ToneSet tones = 1, tonic_tones = 0;
      std::vector<ChordDescription> chords;

      void compute_chords(const class Config&);
    };

  protected:
    const Description& _desc;
    uint8_t _key;
    ToneSet _tones;

    inline Scale(const Description& desc, uint8_t key) :
      _desc(desc), _key(key), _tones(tone_set_shift(desc.tones, key)) {
    }

  public:
    inline std::string full_name() const { return std::string(key_name(_key)) + " " + _desc.name; }
    inline const std::string& name() const { return _desc.name; }
    inline uint8_t key() const { return _key; }
    inline ToneSet tones() const { return _tones; }

    Chord chord_by_tonicity(float ratio) const;
    Chord tonic_chord() const;
    Chord subdominant_chord() const;
    Chord dominant_chord() const;

    friend class Config;
  };
}
