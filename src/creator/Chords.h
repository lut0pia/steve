#pragma once

#include "Creator.h"

namespace steve {
  class Chords : public Creator {
  private:
    uint32_t _md;
  public:
    inline Chords(const Music& m, int mintime = 0, int maxtime = 8)
      : Creator(m, mintime, maxtime), _md(Rand::next(0, NoteValue::eighth)) {}
    Notes get(size_t start, size_t size) const override;
    const char* name() const override { return "Chords"; }
  };
}
