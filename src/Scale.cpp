#include "Scale.h"

#include <algorithm>
#include <cassert>
#include <random>

#include "Config.h"
#include "Rand.h"

using namespace steve;

void ScaleDescription::compute_chords(const Config& instance) {
  // Gather all chords inside scale and find tonic tones
  chords.clear();
  tonic_tones = 0;
  for(const Chord& chord : instance.get_chords_inside(tones)) {
    if(chord.key == 0 && chord.tones > tonic_tones) {
      tonic_tones = chord.tones;
    }
    chords.push_back({chord});
  }

  // Compute chord tonicity
  // Exact tonic tones increase score
  // Close tonic tones decrease score
  for(ScaleChordDescription& desc : chords) {
    for(uint8_t i = 0; i < 12; i++) {
      const uint16_t tone = 1 << i;
      if(desc.chord.tones & tone) {
        if(tonic_tones & tone) {
          desc.tonicity += 1.f;
        } else if((tonic_tones & tone_set_shift(tone, 1)) ||
                  (tonic_tones & tone_set_shift(tone, 11))) {
          desc.tonicity -= 1.f;
        }
      }
    }
  }

  // Sort chords by increasing tonicity
  std::sort(chords.begin(), chords.end(), [](const ScaleChordDescription& a, const ScaleChordDescription& b) {
    return a.tonicity < b.tonicity;
  });
}

Chord Scale::chord_by_tonicity(float ratio) const {
  const uintptr_t index = clamp<uintptr_t>(ratio * desc->chords.size(), 0, desc->chords.size() - 1);
  return desc->chords[index].chord.shifted(key);
}

Chord Scale::tonic_chord() const {
  std::exponential_distribution<float> dist(0.5f);
  const uintptr_t index = clamp<uintptr_t>(desc->chords.size() - dist(Rand::generator) - 1, 0, desc->chords.size() - 1);
  return desc->chords[index].chord.shifted(key);
}
Chord Scale::subdominant_chord() const {
  return chord_by_tonicity(Rand::next_normal());
}
Chord Scale::dominant_chord() const {
  std::exponential_distribution<float> dist(0.5f);
  const uintptr_t index = clamp<uintptr_t>(dist(Rand::generator), 0, desc->chords.size() - 1);
  return desc->chords[index].chord.shifted(key);
}
