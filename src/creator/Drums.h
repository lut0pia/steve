#pragma once

#include "Creator.h"

namespace Steve{
  class Drums : public Creator{
  public:
    inline Drums() : Creator(9,3,6){}
    Notes get(size_t start,size_t size,const std::vector<std::set<uint8_t>>& tones) const override;
  };
}
