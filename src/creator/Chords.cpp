#include "Chords.h"

using namespace steve;

Notes Chords::get(size_t start, size_t size) const {
  Notes notes;
  size_t i(0);
  while(i<size) {
    size_t d(time(i, size));
    std::vector<std::set<uint8_t>> chords(Chord::in(_scale, _music.tones_at(start+i, d)));
    while(chords.empty()) {
      if(!(d /= 2)) break;
      chords = Chord::in(_scale, _music.tones_at(start+i, d));
    }
    if(d>=pow(2, _mintime)) {
      const std::set<uint8_t>& chord(Rand::in(chords));
      uint8_t octave(_instrument.random_octave());
      int j(0);
      for(auto&& tone : chord) {
        if(d-j<=bar_ticks) add_note(notes, _channel, octave+tone, i+j, d-j);
        j += _md;
      }
      i += d;
    } else i++;
  }
  return notes;
}
