#include "Config.h"

#include <random>

using namespace steve;

void Config::compute_cache() {
  for(auto& scale : _scales) {
    scale.compute_chords(*this);
  }
}

std::vector<Chord> Config::get_chords_inside(ToneSet tones) const {
  std::vector<Chord> chords;
  for(const auto& desc : _chords) {
    for(int key(0); key < 12; key++) {
      const Chord shifted_chord(desc, key);
      if((tones | shifted_chord._tones) == tones) { // All chord tones are in the toneset
        chords.push_back(shifted_chord);
      }
    }
  }
  return chords;
}

Scale Config::get_random_scale() const {
  return Scale(Rand::in(_scales), Rand::next(0, 11));
}

const Instrument* Config::get_random_instrument() const {
  return &Rand::in(_instruments);
}
