#pragma once

#include <cstdint>
#include <vector>

#include "Chord.h"
#include "Instrument.h"
#include "Scale.h"

namespace steve {
  class Config {
  protected:
    uint32_t min_tempo, max_tempo;
    std::vector<std::shared_ptr<ChordDescription>> _chords;
    std::vector<std::shared_ptr<ScaleDescription>> _scales;
    std::vector<std::shared_ptr<Instrument>> _instruments;

  public:
    void compute_cache();

    uint32_t get_random_tempo() const;
    std::vector<Chord> get_chords_inside(ToneSet tones) const;
    Scale get_random_scale() const;
    std::shared_ptr<const Instrument> get_random_instrument() const;
  };
}
