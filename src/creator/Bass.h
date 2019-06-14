#pragma once

#include "Creator.h"

namespace steve {
  class Bass : public Creator {
  public:
    Bass(Music*);
    Notes get(size_t start, size_t size) const override;
    const char* name() const override { return "Bass"; }
    std::set<uint8_t> choose_note_from_chord(const ToneSet& chord) const;
  };
}
