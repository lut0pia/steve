#include "Scale.h"

#include <algorithm>

using namespace steve;

std::vector<Scale::Description> Scale::_descriptions;

Scale::Description::Description(const char* name, std::initializer_list<uint8_t> tone_list)
  : name(name) {
  for(uint8_t tone : tone_list) {
    tones |= 1 << tone;
  }

  // Gather all chords inside scale and find tonic tones
  for(const Chord& chord : Chord::chords_inside(tones)) {
    if(chord.key() == 0) {
      tonic_tones |= chord.tones();
    }
    chords.push_back({chord});
  }

  // Compute chord tonicity
  // Exact tonic tones increase score
  // Close tonic tones decrease score
  for(ChordDescription& desc : chords) {
    for(uint8_t i = 0; i < 12; i++) {
      const uint16_t tone = 1 << i;
      if(desc.chord.tones() & tone) {
        if(tonic_tones & tone) {
          desc.tonicity += 1.f;
        } else if((tonic_tones & tone_set_shift(tone, 1)) ||
          (tonic_tones & tone_set_shift(tone, 11))) {
          desc.tonicity -= 1.f;
        }
      }
    }
  }

  // Sort chords by decreasing tonicity
  std::sort(chords.begin(), chords.end(), [](const ChordDescription& a, const ChordDescription& b) {
    return a.tonicity > b.tonicity;
  });
}
