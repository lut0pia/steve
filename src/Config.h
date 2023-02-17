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

    std::vector<std::shared_ptr<ChordDescription>> _allowed_chords;
    std::vector<std::shared_ptr<ScaleDescription>> _allowed_scales;
    std::vector<std::shared_ptr<Instrument>> _allowed_instruments;

  public:
    void compute_cache();

    uint32_t get_random_tempo() const;
    std::vector<Chord> get_chords_inside(ToneSet tones) const;
    std::vector<Chord> get_chord_progression(const Scale&) const;
    Scale get_random_scale() const;
    std::shared_ptr<const Instrument> get_random_instrument() const;
  };
}
