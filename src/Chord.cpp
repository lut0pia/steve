#include "Chord.h"

using namespace steve;

std::vector<ToneSet> Chord::_chords;

ToneSet Chord::possibilities(const ToneSet& scale, const ToneSet& played) {
  ToneSet wtr;
  for(auto&& chord : _chords)
    for(int key(0); key<12; key++)
      if(steve::merge(scale, shift(chord, -key)).size()==scale.size()) // All chord tones are in scale
        if(steve::merge(shift(chord, -key), played).size()==chord.size()) // All played tones are in chord
          wtr = steve::merge(wtr, shift(chord, -key));
  return wtr;
}
std::vector<ToneSet> Chord::in(const ToneSet& scale, const ToneSet& played) {
  std::vector<ToneSet> wtr;
  for(auto&& chord : _chords)
    for(int key(0); key<12; key++) {
      ToneSet shiftedChord(shift(chord, -key));
      if(steve::merge(scale, shiftedChord).size()==scale.size()) // All chord tones are in scale
        if(steve::merge(shiftedChord, played).size()==chord.size()) // All played tones are in chord
          wtr.push_back(shiftedChord);
    }
  return wtr;
}
bool Chord::harmony(const ToneSet& played) {
  ToneSet tones(octave_tones(played));
  for(auto&& chord : _chords)
    for(int key(0); key<12; key++)
      if(steve::merge(shift(chord, -key), tones).size()==shift(chord, -key).size()) // All played tones are in chord
        return true;
  return false;
}
