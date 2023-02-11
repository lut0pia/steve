#pragma once

#include <initializer_list>
#include <string>
#include <vector>

#include "Chord.h"
#include "Steve.h"

namespace steve {
  struct ScaleChordDescription {
    Chord chord;
    float tonicity = 0;
  };
  struct ScaleDescription {
    std::string name;
    ToneSet tones = 1, tonic_tones = 0;
    std::vector<ScaleChordDescription> chords;

    void compute_chords(const class Config&);
  };
  struct Scale {
    std::shared_ptr<const ScaleDescription> desc;
    uint8_t key;
    ToneSet tones;

    inline std::string full_name() const { return std::string(key_name(key)) + " " + desc->name; }

    Chord chord_by_tonicity(float ratio) const;
    Chord tonic_chord() const;
    Chord subdominant_chord() const;
    Chord dominant_chord() const;
  };
}
