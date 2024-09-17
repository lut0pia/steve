#pragma once
#pragma once

#include "../Music.h"

namespace steve {
  class JsonOutput {
  public:
    static void write(const Music&, std::ostream&);
  };
}
