#include "Config.h"

#include "Rand.h"

using namespace steve;

void Config::compute_cache() {
  // This needs to happen before computing scale chords
  _chords.compute_cache();
  for(auto& scale : _scales.get_all()) {
    scale->compute_chords(*this);
  }

  _scales.compute_cache();
  _instruments.compute_cache();
}

uint32_t Config::get_random_tempo() const {
  const float tempo_range = max_tempo - min_tempo;
  const uint32_t full_precision_tempo = uint32_t(tempo_range * Rand::next_normal()) + min_tempo;
  return (full_precision_tempo / 5) * 5;
}

std::vector<Chord> Config::get_chords_inside(ToneSet tones) const {
  std::vector<Chord> chords;
  for(const auto& desc : _chords.get_allowed()) {
    for(int key(0); key < 12; key++) {
      const Chord shifted_chord(desc, key);
      if((tones | shifted_chord.tones) == tones) { // All chord tones are in the toneset
        chords.push_back(shifted_chord);
      }
    }
  }
  return chords;
}
std::vector<Chord> Config::get_chord_progression(const Scale& scale) const {
  std::vector<Chord> chords;
  chords.push_back(scale.tonic_chord());
  chords.push_back(scale.subdominant_chord());
  chords.push_back(scale.subdominant_chord());
  chords.push_back(scale.dominant_chord());
  return chords;
}

Scale Config::get_random_scale() const {
  Scale scale;
  scale.desc = Rand::in(_scales.get_allowed());
  scale.key = Rand::next(0, 11);
  scale.tones = tone_set_shift(scale.desc->tones, scale.key);
  return scale;
}

std::shared_ptr<const Instrument> Config::get_random_instrument() const {
  return Rand::in(_instruments.get_allowed());
}
