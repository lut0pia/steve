#pragma once

#include <cstdint>
#include <string>

namespace steve {
  struct Instrument {
    std::string name;
    uint8_t midi_id, min_tone, max_tone;
  };
}
