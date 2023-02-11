#pragma once

#include <cstdint>

#include "ItemDescription.h"

namespace steve {
  struct Instrument : ItemDescription{
    uint8_t midi_id, min_tone = 0, max_tone = 127;
  };
}
