#pragma once

#include "Creator.h"

namespace steve {
  class Arpeggio : public Creator {
  private:
    uint32_t _md;
  public:
    Arpeggio(Music*);
    Notes get(size_t start, size_t size) const;
    const char* name() const override { return "Arpeggio"; }
  };
}
