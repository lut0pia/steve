#include "Config.h"

#include "Rand.h"

using namespace steve;

void Config::compute_cache() {
  for(auto& scale : _scales) {
    scale->compute_chords(*this);
  }
}

uint32_t Config::get_random_tempo() const {
  const float tempo_range = max_tempo - min_tempo;
  const uint32_t full_precision_tempo = uint32_t(tempo_range * Rand::next_normal()) + min_tempo;
  return (full_precision_tempo / 5) * 5;
}

std::vector<Chord> Config::get_chords_inside(ToneSet tones) const {
  std::vector<Chord> chords;
  for(const auto& desc : _chords) {
    for(int key(0); key < 12; key++) {
      const Chord shifted_chord(desc, key);
      if((tones | shifted_chord.tones) == tones) { // All chord tones are in the toneset
        chords.push_back(shifted_chord);
      }
    }
  }
  return chords;
}

Scale Config::get_random_scale() const {
  Scale scale;
  scale.desc = Rand::in(_scales);
  scale.key = Rand::next(0, 11);
  scale.tones = tone_set_shift(scale.desc->tones, scale.key);
  return scale;
}

std::shared_ptr<const Instrument> Config::get_random_instrument() const {
  return Rand::in(_instruments);
}
