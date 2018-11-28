#pragma once

#include "Creator.h"

namespace steve {
  class Drums : public Creator {
  public:
    inline Drums(const Music& m) : Creator(m, 3, 6) {
      _channel = 9;
    }
    Notes get(size_t start, size_t size) const override;
    const char* name() const override { return "Drums"; }
  };
}
