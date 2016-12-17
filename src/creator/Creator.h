#pragma once

#include "../Chord.h"
#include "../Steve.h"

namespace Steve {
  class Creator {
  protected:
    uint8_t _channel;
    int _mintime,_maxtime;
  public:
    inline Creator(uint8_t channel,int mintime = 0,int maxtime = 8)
      : _channel(channel),_mintime(mintime),_maxtime(maxtime) {}
    virtual ~Creator() {}
    virtual Notes get(size_t start,size_t size,const std::vector<std::set<uint8_t>>& tones) const = 0;

    size_t time(size_t i,size_t size) const;
    std::set<uint8_t> tonesAt(const std::vector<std::set<uint8_t>>& tones,size_t start,size_t size) const;
  };
}
