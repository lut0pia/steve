#pragma once

#include <cstdint>
#include <vector>

#include "Chord.h"
#include "Instrument.h"
#include "Scale.h"

namespace steve {
  class Config {
  protected:
    std::vector<std::shared_ptr<Chord::Description>> _chords;
    std::vector<Scale::Description> _scales;
    std::vector<Instrument> _instruments;

  public:
    void compute_cache();
    std::vector<Chord> get_chords_inside(ToneSet tones) const;
    Scale get_random_scale() const;
    const Instrument* get_random_instrument() const;
  };
}
