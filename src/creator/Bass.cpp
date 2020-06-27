#include "Bass.h"

using namespace steve;

Bass::Bass(Music* music) : Creator(music) {}
Notes Bass::get(size_t start, size_t size) const {
  Notes notes;
  uintptr_t i(0);
  while(i < size) {
    const size_t d(bar_ticks);
    std::set<uint8_t> tones(choose_note_from_chord(_music->tones_at(start + i, d)));

    uint8_t tone(Rand::in(tones));
    add_note(notes, _channel, tone, i, d);

    i += d;
  }
  return notes;
}
bool Bass::is_valid_instrument(const Instrument& instrument) const {
  return instrument.min_tone() <= 35;
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