#include "Chords.h"

using namespace steve;

Chords::Chords(Music* music) : ChordBasedCreator(music) {}
Notes Chords::get(size_t start, size_t size) const {
  Notes notes;
  uintptr_t i(0);
  while(i < size) {
    size_t d(bar_ticks);
    if(d >= (1 << _min_time)) {
      std::vector<uint8_t> tones(chord_for(start + i, d));

      for(uint8_t tone : tones) {
        add_note(notes, _channel, tone, i, d, 100);
      }

      i += d;
    } else i++;
  }
  return notes;
}
