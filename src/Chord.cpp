#include "Chord.h"

using namespace Steve;

std::vector<std::set<uint8_t> > Chord::_chords;

std::set<uint8_t> Chord::possibilities(const std::set<uint8_t>& scale,const std::set<uint8_t>& played){
  std::set<uint8_t> wtr;
  for(auto&& chord : _chords)
    for(int key(0); key<12; key++)
      if(Steve::merge(scale,shift(chord,-key)).size()==scale.size()) // All chord tones are in scale
        if(Steve::merge(shift(chord,-key),played).size()==chord.size()) // All played tones are in chord
          wtr = Steve::merge(wtr,shift(chord,-key));
  return wtr;
}
std::vector<std::set<uint8_t> > Chord::in(const std::set<uint8_t>& scale,const std::set<uint8_t>& played){
  std::vector<std::set<uint8_t> > wtr;
  for(auto&& chord : _chords)
    for(int key(0); key<12; key++){
      std::set<uint8_t> shiftedChord(shift(chord,-key));
      if(Steve::merge(scale,shiftedChord).size()==scale.size()) // All chord tones are in scale
        if(Steve::merge(shiftedChord,played).size()==chord.size()) // All played tones are in chord
          wtr.push_back(shiftedChord);
    }
  return wtr;
}
bool Chord::harmony(const std::set<uint8_t>& played){
  std::set<uint8_t> tones(octaveTones(played));
  for(auto&& chord : _chords)
    for(int key(0); key<12; key++)
      if(Steve::merge(shift(chord,-key),tones).size()==shift(chord,-key).size()) // All played tones are in chord
        return true;
  return false;
}
