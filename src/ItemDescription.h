#pragma once

#include <cstdint>
#include <string>

namespace steve {
  struct ItemDescription {
    std::string name;
    bool blacklisted = false, whitelisted = false;
  };
}
