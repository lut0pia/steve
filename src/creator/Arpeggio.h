#pragma once

#include "Creator.h"

namespace steve {
  class Arpeggio : public Creator {
  private:
    uint32_t _md;
  public:
    inline Arpeggio(const Music& m, int mintime = 0, int maxtime = 8)
      : Creator(m, mintime, maxtime), _md(pow(2, Rand::next(3, mintime))) {}
    Notes get(size_t start, size_t size) const;
    const char* name() const override { return "Arpeggio"; }
  };
}
