#include "Chord.h"

using namespace steve;

std::vector<ToneSet> Chord::_chords;

std::vector<ToneSet> Chord::chords_inside(const ToneSet& scale) {
  std::vector<ToneSet> wtr;
  for(const ToneSet& chord : _chords) {
    for(int key(0); key < 12; key++) {
      const ToneSet shifted_chord(tone_set_shift(chord, key));
      if((scale | shifted_chord) == scale) { // All chord tones are in scale
        wtr.push_back(shifted_chord);
      }
    }
  }
  return wtr;
}
void Chord::add(const std::initializer_list<uint8_t>& tones) {
  ToneSet chord(0);
  for(uint8_t tone : tones) {
    chord |= 1 << tone;
  }
  _chords.push_back(chord);
}