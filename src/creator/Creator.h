#pragma once

#include "../Chord.h"
#include "../Music.h"
#include "../Steve.h"

namespace steve {
  class Creator {
  protected:
    const Music& _music;
    uint8_t _channel;
    const Instrument& _instrument;
    const Scale& _scale;
    int _mintime, _maxtime;
  public:
    inline Creator(const Music& m, int mintime = 0, int maxtime = 8)
      : _music(m), _channel(uint8_t(m.instruments().size()-1)),
      _instrument(*m.instruments().back()), _scale(m.scale()),
      _mintime(mintime), _maxtime(maxtime) {
    }
    virtual ~Creator() {}
    virtual Notes get(size_t start, size_t size) const = 0;
    virtual const char* name() const = 0;
    virtual inline float repetition() const { return Rand::next_float(); }

    size_t time(size_t i, size_t size) const;
  };
}
