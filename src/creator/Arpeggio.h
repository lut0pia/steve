#pragma once

#include "Tonal.h"

namespace Steve {
  class Arpeggio : public Tonal {
  private:
    uint32_t _md;
  public:
    inline Arpeggio(uint8_t channel,const Scale& scale,const Instrument& instrument,int mintime = 0,int maxtime = 8)
      : Tonal(channel,scale,instrument,mintime,maxtime),_md(pow(2,Rand::next(3,mintime))) {}
    Notes get(size_t start,size_t size,const std::vector<std::set<uint8_t>>& base) const;
  };
}
