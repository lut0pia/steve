#include "JsonOutput.h"

#include <iostream>

#include "../cfg/Instrument.h"
#include "../ext/json.hpp"

using namespace steve;

void steve::JsonOutput::write(const Music& music, std::ostream& s) {
  using json = nlohmann::json;

  const Scale& scale = music.get_scale();

  std::vector<Chord> scale_chords = scale.get_chords();
  json scale_chords_json;
  for(const Chord& chord : scale_chords) {
    scale_chords_json.push_back({
      {"name", chord.desc->name},
      {"suffix", chord.desc->suffix},
      {"key", chord.key},
      {"tones", chord.tones},
      {"degree", scale.get_degree_for_tone(chord.key)},
      {"uppercase", chord.desc->uppercase},
    });
  }

  json progression;
  for(const Chord& chord : music.get_progression()) {
    const auto scale_chord_it = std::find_if(scale_chords.cbegin(), scale_chords.cend(),
      [&chord](const Chord& scale_chord) {
        return chord.desc == scale_chord.desc && chord.key == scale_chord.key;
      });
    progression.push_back(scale_chord_it - scale_chords.cbegin());
  }

  json parts;
  for(const auto& creator : music.get_creators()) {
    const auto instrument = *creator->get_instrument();
    json phrases;
    for(const Phrase& phrase : creator->get_phrases()) {
      phrases.push_back({{"size", phrase.size / music.get_bar_ticks()}});
    }
    json phrase_list;
    for(const size_t& phrase : creator->get_phrase_list()) {
      phrase_list.push_back(phrase);
    }
    json part{
      {"creator", creator->name()},
      {"instrument",
        {
          {"name", instrument.name},
          {"index", instrument.midi_id},
        }},
      {"phrases", phrases},
      {"phrase_list", phrase_list},
    };
    parts.push_back(part);
  }

  json root{
    {"scale",
      {
        {"name", scale.desc->name},
        {"key", scale.key},
        {"tones", scale.tones},
        {"chords", scale_chords_json},
      }},
    {"tempo", music.get_tempo()},
    {"signature",
      {
        {"beats_per_bar", music.get_signature().beats_per_bar},
        {"beat_value", music.get_signature().beat_value_numeral},
      }},
    {"duration", music.get_duration()},
    {"progression", progression},
    {"parts", parts},
  };

  s << root;
}