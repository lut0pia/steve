#pragma once

#include <string>
#include <vector>

#include "Chord.h"
#include "ItemDescription.h"
#include "Steve.h"

namespace steve {
  struct ScaleChordDescription {
    Chord chord;
    float tonicity = 0;
  };
  struct ScaleDescription : public ItemDescription {
    ToneSet tones = 1, tonic_tones = 0;
    std::vector<ScaleChordDescription> chords;

    void compute_chords(const class Config&);
  };
  struct Scale {
    std::shared_ptr<const ScaleDescription> desc;
    uint8_t key = 0;
    ToneSet tones = 0;

    inline Scale(const std::shared_ptr<const ScaleDescription>& d, uint8_t k)
      : desc(d), key(k), tones(tone_set_shift(d->tones, k)) {}

    inline std::string full_name() const { return std::string(key_name(key)) + " " + desc->name; }

    // Zero-based
    uint8_t get_degree_for_tone(uint8_t tone) const;
    Chord chord_by_tonicity(float ratio) const;
    Chord tonic_chord() const;
    Chord subdominant_chord() const;
    Chord dominant_chord() const;
  };
}
