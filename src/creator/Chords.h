#pragma once

#include "Creator.h"

namespace steve {
  class Chords : public Creator {
  private:
    uint32_t _md;
  public:
    Chords(Music*);
    Notes get(size_t start, size_t size) const override;
    const char* name() const override { return "Chords"; }
  };
}
