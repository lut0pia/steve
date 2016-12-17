#pragma once

#include "Creator.h"
#include "../Scale.h"
#include "../Instrument.h"

namespace Steve {
  class Tonal : public Creator {
  protected:
    Scale _scale;
    Instrument _instrument;
  public:
    inline Tonal(uint8_t channel,const Scale& scale,const Instrument& instrument,int mintime,int maxtime)
      : Creator(channel,mintime,maxtime),_scale(scale),_instrument(instrument) {}
    virtual ~Tonal() {}
  };
}
