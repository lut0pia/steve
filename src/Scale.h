#pragma once

#include <string>
#include <vector>
#include <initializer_list>
#include "Chord.h"
#include "Rand.h"
#include "Steve.h"

namespace steve {
  class Scale {
  protected:
    struct ChordDescription {
      Chord chord;
      float tonicity = 0;
    };
    struct Description {
      Description(const char* name, std::initializer_list<uint8_t> tone_list);
      std::string name;
      ToneSet tones = 0, tonic_tones = 0;
      std::vector<ChordDescription> chords;
    };

    static std::vector<Description> _descriptions;

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

    static inline void add(const char* name, std::initializer_list<uint8_t> tone_list) { _descriptions.push_back(Description(name, tone_list)); }
    static inline Scale random() { return Scale(Rand::in(_descriptions), Rand::next(0, 11)); }
  };
}
