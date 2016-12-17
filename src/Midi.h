#pragma once

#include <cstdint>

#define L_ToMidiNote(n) ((n)+24) // Shift the note to the right midi equivalent

namespace Steve {
  class Midi {
  private:
    void* _device;
    void send(uint8_t msgData[4]);

  public:
    Midi();
    ~Midi();
    void setInstrument(uint8_t channel,uint8_t instrument);
    void playNote(uint8_t channel,uint8_t noteNumber,uint8_t velocity = 127);
    void stopNote(uint8_t channel,uint8_t noteNumber);
    void stopAll(uint8_t channel);
    void stopAll();
  };
}
