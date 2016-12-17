#include "Midi.h"

#include <windows.h>
#include <mmsystem.h>

using namespace Steve;

Midi::Midi(){
  midiOutOpen((HMIDIOUT*)&_device,0,0,0,CALLBACK_NULL);
}
Midi::~Midi(){
  midiOutClose((HMIDIOUT)_device);
}

void Midi::send(uint8_t msgData[4]) {
  union { unsigned long word; uint8_t data[4]; } message;
  message.data[0] = msgData[0];
  message.data[1] = msgData[1];
  message.data[2] = msgData[2];
  message.data[3] = msgData[3];
  midiOutShortMsg((HMIDIOUT)_device,message.word);
}