#include "JsonOutput.h"

#include <iostream>

#include "../ext/json.hpp"

using namespace steve;

void steve::JsonOutput::write(const Music& music, std::ostream& s) {
  using json = nlohmann::json;
  json root{
    {"scale", std::string(key_name(music.get_scale().key)) + " " + music.get_scale().desc->name},
    {"tempo", music.get_tempo()},
    {"signature",
      {
        {"beats_per_bar", music.get_signature().beats_per_bar},
        {"beat_value", music.get_signature().beat_value_numeral},
      }},
    {"duration", music.get_duration()},
  };

  json& chord_progression = root["chord_progression"];
  for(const Chord& chord : music.get_chord_progression()) {
    chord_progression.push_back({
      {"degree", music.get_scale().get_degree_string_for_chord(chord)},
      {"chord", chord.to_short_string()},
    });
  }
  s << root;
}