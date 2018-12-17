#pragma once

#include "../Chord.h"
#include "../Music.h"
#include "../Steve.h"

namespace steve {
  class Creator {
  protected:
    Music* _music;
    const Instrument* _instrument;
    int _mintime, _maxtime;
    float _repetition;
    uint8_t _channel;
  public:
    Creator(Music*);
    virtual ~Creator() {}
    virtual Notes compose();
    virtual Notes get(size_t start, size_t size) const = 0;
    virtual const char* name() const = 0;
    virtual void write_txt(std::ostream&) const;

    inline const Instrument* instrument() const { return _instrument; }

    size_t time(size_t i, size_t size) const;
  };
}
