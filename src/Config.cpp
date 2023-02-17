#include "Config.h"

#include "Rand.h"

using namespace steve;

template <class T>
void compute_allowed_items(const std::vector<std::shared_ptr<T>>& all, std::vector<std::shared_ptr<T>>& allowed) {
  bool use_whitelist = false;

  for(const auto& item : all) {
    if(item->whitelisted) {
      use_whitelist = true;
    }
  }

  for(const auto& item : all) {
    if(!item->blacklisted && (!use_whitelist || item->whitelisted)) {
      allowed.push_back(item);
    }
  }
}

void Config::compute_cache() {
  // This needs to happen before computing scale chords
  compute_allowed_items(_chords, _allowed_chords);
  for(auto& scale : _scales) {
    scale->compute_chords(*this);
  }

  compute_allowed_items(_scales, _allowed_scales);
  compute_allowed_items(_instruments, _allowed_instruments);
}

uint32_t Config::get_random_tempo() const {
  const float tempo_range = max_tempo - min_tempo;
  const uint32_t full_precision_tempo = uint32_t(tempo_range * Rand::next_normal()) + min_tempo;
  return (full_precision_tempo / 5) * 5;
}

std::vector<Chord> Config::get_chords_inside(ToneSet tones) const {
  std::vector<Chord> chords;
  for(const auto& desc : _allowed_chords) {
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
  scale.desc = Rand::in(_allowed_scales);
  scale.key = Rand::next(0, 11);
  scale.tones = tone_set_shift(scale.desc->tones, scale.key);
  return scale;
}

std::shared_ptr<const Instrument> Config::get_random_instrument() const {
  return Rand::in(_allowed_instruments);
}
