#include "Chord.h"

using namespace steve;

std::vector<ToneSet> Chord::_chords;

ToneSet Chord::tones_in_harmony(const ToneSet& scale, const ToneSet& played) {
  ToneSet wtr(0);
  for(const ToneSet& chord : _chords) {
    for(int key(0); key<12; key++) {
      const ToneSet shifted_chord(shift(chord, -key));
      if(scale|shifted_chord == scale // All chord tones are in scale
        && shifted_chord|played == shifted_chord) {// All played tones are in chord
        wtr |= shifted_chord;
      }
    }
  }
  return wtr;
}
std::vector<ToneSet> Chord::chords_in_harmony(const ToneSet& scale, const ToneSet& played) {
  std::vector<ToneSet> wtr;
  for(const ToneSet& chord : _chords) {
    for(int key(0); key<12; key++) {
      const ToneSet shifted_chord(shift(chord, -key));
      if((scale|shifted_chord) == scale // All chord tones are in scale
        && (shifted_chord|played) == shifted_chord) {// All played tones are in chord
        wtr.push_back(shifted_chord);
      }
    }
  }
  return wtr;
}
bool Chord::harmony(const ToneSet& played) {
  for(const ToneSet& chord : _chords) {
    for(int key(0); key<12; key++) {
      const ToneSet shifted_chord(shift(chord, -key));
      if(shifted_chord|played == shifted_chord) { // All played tones are in chord
        return true;
      }
    }
  }
  return false;
}
void Chord::add(const std::initializer_list<uint8_t>& tones) {
  ToneSet chord(0);
  for(uint8_t tone : tones) {
    chord |= 1<<tone;
  }
  _chords.push_back(chord);
}