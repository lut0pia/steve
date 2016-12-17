#include "Chords.h"

using namespace Steve;

Notes Chords::get(size_t start,size_t size,const std::vector<std::set<uint8_t>>& base) const {
  Notes notes;
  size_t i(0);
  while(i<size) {
    size_t d(time(i,size));
    std::vector<std::set<uint8_t>> chords(Chord::in(_scale,tonesAt(base,start+i,d)));
    while(chords.empty()) {
      if(!(d /= 2)) break;
      chords = Chord::in(_scale,tonesAt(base,start+i,d));
    }
    if(d>=pow(2,_mintime)) {
      const std::set<uint8_t>& chord(Rand::in(chords));
      uint8_t octave(_instrument.randomOctave());
      int j(0);
      for(auto&& tone : chord) {
        if(d-j<=barTicks) addNote(notes,_channel,octave+tone,i+j,d-j);
        j += md;
      }
      i += d;
    } else i++;
  }
  return notes;
}
