#pragma once

#include "../Music.h"

namespace steve {
  class MidiOutput {
  public:
    static void write(const Music&, std::ostream&);
  };
}
