#pragma once

#include "Creator.h"

namespace steve {
  class Melody : public Creator {
  private:
    int _maxinter; // Maximum melodic interval
  public:
    inline Melody(const Music& m, uint32_t mintime = 0, uint32_t maxtime = 8, uint32_t maxinter = 0xFF)
      : Creator(m, mintime, maxtime), _maxinter(maxinter) {}
    Notes get(size_t start, size_t size) const override;
    const char* name() const override { return "Melody"; }
    std::set<uint8_t> notes_in_harmony(const Tones& base, size_t start, size_t size, uint8_t lasttone) const;
    std::set<uint8_t> near_enough(const std::set<uint8_t>&, uint8_t lasttone) const;
  };
}
