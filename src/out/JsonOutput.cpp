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
    assert(scale_chord_it != scale_chords.end());
    progression.push_back(scale_chord_it - scale_chords.cbegin());
  }

  json tracks;
  for(const auto& creator : music.get_creators()) {
    const auto instrument = *creator->get_instrument();
    json patterns;
    for(const Pattern& pattern : creator->get_patterns()) {
      json note_starts, note_durations, note_tones;
      for(const auto& note : pattern.notes) {
        note_starts.push_back(note.first);
        note_durations.push_back(note.second.duration);
        note_tones.push_back(note.second.tone);
      }
      patterns.push_back({
        {"note_starts", note_starts},
        {"note_durations", note_durations},
        {"note_tones", note_tones},
        {"size", pattern.size / music.get_bar_ticks()},
      });
    }
    json pattern_list;
    for(const size_t& pattern : creator->get_pattern_list()) {
      pattern_list.push_back(pattern);
    }
    json track{
      {"creator", creator->name()},
      {"patterns", patterns},
      {"pattern_list", pattern_list},
      {"channel", creator->get_channel()},
      {"min_tone", creator->get_min_tone()},
      {"max_tone", creator->get_max_tone()},
    };
    if(creator->get_channel() != 9) {
      track["instrument"] = {
        {"name", instrument.name},
        {"index", instrument.midi_id},
      };
    }
    tracks.push_back(track);
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
    {"beat_count", music.get_beat_count()},
    {"bar_ticks", music.get_bar_ticks()},
    {"progression", progression},
    {"tracks", tracks},
  };

  s << root;
}