#include "Arpeggio.h"

using namespace steve;

Arpeggio::Arpeggio(Music* music) : Creator(music), _md(1<<Rand::next(NoteValue::sixteenth, _min_time)) {}
Notes Arpeggio::get(size_t start, size_t size) const {
  Notes notes;
  uint32_t i(0);
  while(i<size) {
    uint32_t d(time(i, size));
    std::vector<ToneSet> chords(Chord::chords_in_harmony(_music->scale().tones(), _music->tones_at(start+i, d)));
    while(chords.empty()) {
      if(!(d /= 2)) break;
      chords = Chord::chords_in_harmony(_music->scale().tones(), _music->tones_at(start+i, d));
    }
    if(d >= (1<<_min_time)) {
      ToneSet chord(Rand::in(chords));
      uint8_t octave(_instrument->random_octave());

      // Get chord tones in a vector
      std::vector<uint8_t> ordered_tones;
      for(uint8_t t(0); t<12; t++) {
        if(chord & 1<<t) {
          ordered_tones.push_back(t);
        }
      }

      // Randomize those tones (this is terribly ugly)
      std::vector<uint8_t> tones;
      while(!ordered_tones.empty()) {
        uintptr_t i(Rand::next(0ull, ordered_tones.size()-1));
        tones.push_back(ordered_tones[i]);
        ordered_tones.erase(ordered_tones.begin()+i);
      }
      for(uint32_t k(0); k*_md<d; k++)
        add_note(notes, _channel, octave+tones[k%tones.size()], i+k*_md, _md);
      i += d;
    } else i++;
  }
  return notes;
}
