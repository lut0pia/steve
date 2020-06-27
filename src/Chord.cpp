#include "Chord.h"

#include "Scale.h"

using namespace steve;

std::vector<Chord::Description> Chord::_descriptions;

Chord::Description::Description(const char* name, const char* suffix, std::initializer_list<uint8_t> tone_list)
  : name(name), suffix(suffix), tones(0) {
  for(uint8_t tone : tone_list) {
    tones |= 1 << tone;
  }
}

std::string Chord::to_short_string() const {
  return key_name(_key) + _desc.suffix;
}

std::vector<Chord> Chord::chords_inside(const Scale& scale) {
  ToneSet tones(scale.tones());
  std::vector<Chord> wtr;
  for(const Description& desc : _descriptions) {
    for(int key(0); key < 12; key++) {
      const Chord shifted_chord(desc, key);
      if((tones | shifted_chord._tones) == tones) { // All chord tones are in scale
        wtr.push_back(shifted_chord);
      }
    }
  }
  return wtr;
}
void Chord::add(const char* name, const char* suffix, std::initializer_list<uint8_t> tone_list) {
  _descriptions.push_back(Description(name, suffix, tone_list));
}
