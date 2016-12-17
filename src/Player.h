#pragma once

#include <chrono>
#include "Midi.h"
#include "Music.h"

namespace Steve {
  class Player {
  protected:
    Midi _midi;
    uint64_t _tickTime,_lastTick;
    const Music* _music;
    uint32_t _pos;

  public:
    inline Player() : _music(nullptr) {}
    inline bool free() const { return _music == nullptr; }
    void start(const Music*);
    void play();
    void stop();
  };
}
