#include "Chord.h"

#include "Scale.h"

using namespace steve;

std::vector<Chord> Chord::_chords;

Chord::Chord(const char* name, const char* suffix, std::initializer_list<uint8_t> tones)
  : _name(name), _suffix(suffix), _key(0), _tones(0) {
  for(uint8_t tone : tones) {
    _tones |= 1 << tone;
  }
}
std::string Chord::to_short_string() const {
  return key_name(_key) + _suffix;
}

std::vector<Chord> Chord::chords_inside(const Scale& scale) {
  ToneSet tones(scale.tones());
  std::vector<Chord> wtr;
  for(const Chord& chord : _chords) {
    for(int key(0); key < 12; key++) {
      const Chord shifted_chord(chord, key);
      if((tones | shifted_chord._tones) == tones) { // All chord tones are in scale
        wtr.push_back(shifted_chord);
      }
    }
  }
  return wtr;
}
void Chord::add(const Chord& chord) {
  _chords.push_back(chord);
}