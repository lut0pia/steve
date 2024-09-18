#pragma once

#include "../Music.h"

namespace steve {
  class TextOutput {
  public:
    static void write(const Music&, std::ostream&);
  };
}
