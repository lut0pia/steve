#pragma once
#include <functional>

#include "ItemDescription.h"

namespace steve {
  struct CreatorDescription : public ItemDescription {
    std::function<class Creator*(class Music*)> func;
    uint32_t min_count = 0, max_count = 1;
  };
}
