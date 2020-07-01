#include "Chord.h"

#include "Scale.h"

using namespace steve;

std::vector<std::shared_ptr<Chord::Description>> Chord::_descriptions;

std::string Chord::to_short_string() const {
  return key_name(_key) + _desc->suffix;
}

std::vector<Chord> Chord::chords_inside(ToneSet tones) {
  std::vector<Chord> chords;
  for(const auto& desc : _descriptions) {
    for(int key(0); key < 12; key++) {
      const Chord shifted_chord(desc, key);
      if((tones | shifted_chord._tones) == tones) { // All chord tones are in the toneset
        chords.push_back(shifted_chord);
      }
    }
  }
  return chords;
}

void Chord::add(const char* name, const char* suffix, std::initializer_list<uint8_t> tone_list) {
  std::shared_ptr<Description> desc(new Description());
  desc->name = name;
  desc->suffix = suffix;
  for(uint8_t tone : tone_list) {
    desc->tones |= 1 << tone;
  }
  _descriptions.push_back(desc);
}
