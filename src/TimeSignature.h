#pragma once

#include "ItemDescription.h"
#include "Steve.h"

namespace steve {
  struct TimeSignature : public ItemDescription {
    uint32_t beats_per_bar = 4; // Upper numeral
    NoteValue beat_value = NoteValue::quarter;
    uint32_t beat_value_numeral = 4; // Lower numeral

    virtual void compute_cache() override {
      beat_value_numeral = 1 << (uint32_t(NoteValue::whole) - uint32_t(beat_value));
    }
  };
}
