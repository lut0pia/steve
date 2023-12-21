#include "Config.h"

#include <iostream>

#include "Music.h"
#include "Rand.h"
#include "creator/Arpeggio.h"
#include "creator/Bass.h"
#include "creator/Chords.h"
#include "creator/Drums.h"
#include "creator/Melody.h"

using namespace steve;

Config::Config() {
  _signatures.push_back({4, NoteValue::quarter});

  _creators.get_item("Arpeggio")->func = [](Music* music) {
    return new Arpeggio(music);
  };
  _creators.get_item("Bass")->func = [](Music* music) {
    return new Bass(music);
  };
  _creators.get_item("Chords")->func = [](Music* music) {
    return new Chords(music);
  };
  _creators.get_item("Drums")->func = [](Music* music) {
    return new Drums(music);
  };
  _creators.get_item("Melody")->func = [](Music* music) {
    return new Melody(music);
  };
}

void Config::compute_cache() {
  // This needs to happen before computing scale chords
  _chords.compute_cache();
  for(auto& scale : _scales.get_all()) {
    scale->compute_chords(*this);
  }

  _scales.compute_cache();
  _instruments.compute_cache();
  _creators.compute_cache();
}

void Config::list_scales(std::ostream& out) const {
  for(const auto& scale_desc : _scales.get_allowed()) {
    Scale scale(scale_desc, 0);
    out << scale.desc->name << ":" << std::endl;
    for(const auto& chord : scale.desc->chords) {
      out << '\t' << scale.get_degree_string_for_chord(chord) << "\n";
    }
  }
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
  chords.push_back(Rand::in(scale.desc->chords).shifted(scale.key));
  chords.push_back(Rand::in(scale.desc->chords).shifted(scale.key));
  chords.push_back(Rand::in(scale.desc->chords).shifted(scale.key));
  chords.push_back(Rand::in(scale.desc->chords).shifted(scale.key));
  return chords;
}

Scale Config::get_random_scale() const {
  return Scale(_scales.get_random_item(), Rand::next(0, 11));
}

std::shared_ptr<const Instrument> Config::get_random_instrument() const {
  return _instruments.get_random_item();
}
TimeSignature Config::get_random_time_signature() const {
  return Rand::in(_signatures);
}
std::vector<std::shared_ptr<const CreatorDescription>> Config::get_creators() const {
  std::vector<std::shared_ptr<const CreatorDescription>> creators;

  while(creators.empty()) {
    for(const auto creator : _creators.get_allowed()) {
      const uint32_t count = Rand::next(creator->min_count, creator->max_count);
      for(uint32_t i = 0; i < count; i++) {
        creators.push_back(creator);
      }
    }
  }

  return creators;
}
