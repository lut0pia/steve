#include "Chords.h"

using namespace steve;

Chords::Chords(Music* music) : Creator(music), _md(Rand::next(0, NoteValue::eighth)) {}
Notes Chords::get(size_t start, size_t size) const {
  Notes notes;
  size_t i(0);
  while(i<size) {
    size_t d(time(i, size));
    std::vector<ToneSet> chords(Chord::chords_in_harmony(_music->scale().tones(), _music->tones_at(start+i, d)));
    while(chords.empty()) {
      if(!(d /= 2)) break;
      chords = Chord::chords_in_harmony(_music->scale().tones(), _music->tones_at(start+i, d));
    }
    if(d >= (1<<_min_time)) {
      const ToneSet& chord(Rand::in(chords));
      uint8_t octave(_instrument->random_octave());
      int j(0);
      for(uint8_t tone(0); tone<12; tone++) {
        if(chord & 1<<tone) {
          if(d-j<=bar_ticks) {
            // Decreased velocity because chords shouldn't take up all the space
            add_note(notes, _channel, octave+tone, i+j, d-j, 64);
          }
          j += _md;
        }
      }
      i += d;
    } else i++;
  }
  return notes;
}
