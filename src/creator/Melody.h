#pragma once

#include "Tonal.h"

namespace Steve {
  class Melody : public Tonal {
  private:
    int _maxinter; // Maximum melodic interval
  public:
    inline Melody(uint8_t channel,const Scale& scale,const Instrument& instrument,uint32_t mintime = 0,uint32_t maxtime = 8,uint32_t maxinter = 0xFF)
      : Tonal(channel,scale,instrument,mintime,maxtime),_maxinter(maxinter) {}
    Notes get(size_t start,size_t size,const std::vector<std::set<uint8_t>>& tones) const override;
    std::set<uint8_t> possibilities(const std::vector<std::set<uint8_t>>& base,size_t start,size_t size,uint8_t lasttone) const;
    std::set<uint8_t> nearEnough(const std::set<uint8_t>&,uint8_t lasttone) const;
  };
}
