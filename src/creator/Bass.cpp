#include "Bass.h"

#include "../Music.h"

using namespace steve;

Bass::Bass(Music* music) : Creator(music) {}
void Bass::init() {
  Creator::init();

  _min_tone = 36;
  _max_tone = _min_tone + 12;

  _min_time = _max_time = Rand::next(NoteValue::quarter, NoteValue::whole);
}
Notes Bass::get(size_t start, size_t size) const {
  Notes notes;
  uintptr_t i = 0;
  while(i < size) {
    const size_t d = ticks_for(_min_time);
    const Chord& chord = _music->chord_at(start + i);
    const uint8_t tone = _min_tone + chord.key();
    add_note(notes, _channel, tone, i, d);

    i += d;
  }
  return notes;
}
bool Bass::is_valid_instrument(const Instrument& instrument) const {
  return instrument.min_tone() <= 36 && instrument.max_tone() >= 48;
}
std::set<uint8_t> Bass::choose_note_from_chord(const ToneSet& tones) const {
  std::set<uint8_t> notes_in_ambitus;
  for(uint8_t tone(0); tone < 12; tone++) {
    if(tones & (1 << tone)) {
      for(uint8_t t(tone); t <= _max_tone; t += 12) {
        if(t >= _min_tone) {
          notes_in_ambitus.insert(t);
        }
      }
    }
  }
  return notes_in_ambitus;
}