#include "Midi.h"

using namespace Steve;

void Midi::setInstrument(uint8_t channel,uint8_t instrument) {
  uint8_t msgData[4] = {
    0xC0 + (channel%16), // Channel changing instrument
    instrument%128, // New instrument id
    0,0
  };
  send(msgData);
}
void Midi::playNote(uint8_t channel,uint8_t noteNumber,uint8_t velocity) {
  uint8_t msgData[4] = {
    0x90 + (channel%16), // Channel playing note
    noteNumber%128, // Note
    velocity%128, // Velocity
    0
  };
  send(msgData);
}
void Midi::stopNote(uint8_t channel,uint8_t noteNumber) {
  uint8_t msgData[4] = {
    0x80 + (channel%16), // Channel stopping note
    noteNumber%128, // Note
    0,0
  };
  send(msgData);
}
void Midi::stopAll(uint8_t channel) {
  for(uint8_t t(0); t<128; t++)
    stopNote(channel,t);
}
void Midi::stopAll() {
  for(uint8_t c(0); c<16; c++)
    stopAll(c);
}
