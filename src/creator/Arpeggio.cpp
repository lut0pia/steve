#include "Arpeggio.h"

#include <ostream>

#include "../Rand.h"

using namespace steve;

Arpeggio::Arpeggio(Music* music) : ChordBasedCreator(music) {}
void Arpeggio::init() {
  ChordBasedCreator::init();
  _step = Rand::next(NoteValue::sixteenth, _min_time);
}
Notes Arpeggio::get(size_t start, size_t size) const {
  Notes notes;
  uintptr_t i(0);
  while(i < size) {
    size_t d(bar_ticks);
    if(d >= ticks_for(_min_time)) {
      std::vector<uint8_t> tones(chord_for(start + i, d));

      const size_t step_ticks = ticks_for(_step);
      for(uint32_t k(0); k*step_ticks < d; k++) {
        add_note(notes, _channel, tones[k%tones.size()], i + k * step_ticks, step_ticks);
      }

      i += d;
    } else i++;
  }
  return notes;
}

void Arpeggio::write_txt(std::ostream& s) const {
  ChordBasedCreator::write_txt(s);
  s << "\t\tArpeggio step: " << note_value_name(_step) << std::endl;
}
