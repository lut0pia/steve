#pragma once

#include "Tonal.h"

namespace Steve {
  class Chords : public Tonal {
  private:
    uint32_t md;
  public:
    inline Chords(uint8_t channel,const Scale& scale,const Instrument& instrument,int mintime = 0,int maxtime = 8)
      : Tonal(channel,scale,instrument,mintime,maxtime),md(0) {}
    Notes get(size_t start,size_t size,const std::vector<std::set<uint8_t>>& tones) const override;
  };
}
