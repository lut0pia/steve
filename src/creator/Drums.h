#pragma once

#include "Creator.h"

namespace steve {
  class Drums : public Creator {
  public:
    Drums(Music* music);
    Notes get(size_t start, size_t size) const override;
    const char* name() const override { return "Drums"; }
  };
}
