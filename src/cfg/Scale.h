#pragma once

#include <memory>
#include <vector>

#include "../Steve.h"
#include "Chord.h"
#include "ItemDescription.h"

namespace steve {
  struct ScaleDescription : public ItemDescription {
    std::vector<Chord> chords;
    ToneSet tones = 1;

    void compute_chords(const class Config&);
  };
  struct Scale {
    std::shared_ptr<const ScaleDescription> desc;
    uint8_t key = 0;
    ToneSet tones = 0;

    inline Scale(const std::shared_ptr<const ScaleDescription>& d, uint8_t k)
      : desc(d), key(k), tones(tone_set_shift(d->tones, k)) {}

    std::vector<Chord> get_chords() const;
    uint8_t get_degree_for_tone(uint8_t tone) const; // Zero-based
    std::string get_degree_string_for_chord(const Chord& chord) const;
  };
}
