#pragma once

#include <cstdint>
#include <vector>

#include "Chord.h"
#include "ConfigItemList.h"
#include "Instrument.h"
#include "Scale.h"

namespace steve {
  class Config {
  protected:
    uint32_t min_tempo, max_tempo;
    ConfigItemList<ChordDescription> _chords;
    ConfigItemList<ScaleDescription> _scales;
    ConfigItemList<Instrument> _instruments;

  public:
    void compute_cache();

    uint32_t get_random_tempo() const;
    std::vector<Chord> get_chords_inside(ToneSet tones) const;
    std::vector<Chord> get_chord_progression(const Scale&) const;
    Scale get_random_scale() const;
    std::shared_ptr<const Instrument> get_random_instrument() const;
  };
}
