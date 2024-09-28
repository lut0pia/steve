#include "Scale.h"

#include <algorithm>

#include "Config.h"

using namespace steve;

void ScaleDescription::compute_chords(const Config& instance) {
  // Gather all chords inside scale
  chords = instance.get_chords_inside(tones);

  std::sort(chords.begin(), chords.end(), [](const Chord& a, const Chord& b) {
    return a.key < b.key;
  });
}

std::vector<Chord> Scale::get_chords() const {
  std::vector<Chord> chords = desc->chords;
  std::transform(chords.cbegin(), chords.cend(), chords.begin(),
    [this](const Chord& desc_chord) {
      return Chord(desc_chord.desc, (key + desc_chord.key) % 12);
    });
  return chords;
}
uint8_t Scale::get_degree_for_tone(uint8_t tone) const {
  tone = tone % 12;
  uint8_t cmp_tone = key;
  uint8_t degree = 0;
  while(cmp_tone != tone) {
    cmp_tone = (cmp_tone + 1) % 12;
    if((tones & (1 << cmp_tone)) != 0) {
      degree += 1;
    }
  }
  return degree;
}
std::string Scale::get_degree_string_for_chord(const Chord& chord) const {
  return steve::degree_name(get_degree_for_tone(chord.key), chord.desc->uppercase) + chord.desc->suffix;
}
