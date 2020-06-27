#include "Drums.h"

#include "../Rand.h"

using namespace steve;

Drums::Drums(Music* music) : Creator(music) {}
void Drums::init() {
  Creator::init();
  _channel = 9;
  _repetition = 1;
}
Notes Drums::get(size_t start, size_t size) const {
  Notes notes;

  uint32_t layers(Rand::next(2, 5));
  for(uint32_t i(0); i < layers; i++) {
    uint8_t tone(Rand::next(35, 59));
    NoteValue period_value(NoteValue(Rand::next(eighth, whole)));
    uintptr_t period(1 << period_value);
    uintptr_t offset(1 << Rand::next(eighth, period_value));
    if(i == 0 || Rand::next(0, 3) > 0) {
      offset = 0;
    }

    for(uintptr_t j(offset); j < size; j += period) {
      add_note(notes, _channel, tone, j, 1, 100);
    }
  }
  return notes;
}
