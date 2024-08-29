#pragma once

#include "Music.h"

namespace steve {
  namespace midi {
    void write(const Music&, std::ostream&);
  };
}
