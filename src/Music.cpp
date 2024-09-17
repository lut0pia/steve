#include "Music.h"

#include <algorithm>
#include <cassert>
#include <iostream>

#include "Chord.h"
#include "Config.h"
#include "Rand.h"
#include "Scale.h"
#include "creator/Creator.h"

using namespace steve;

Music::Music(const Config& config)
  : _config(config),
    _scale(Scale(_config.get_scales().get_random_item(), Rand::next(0, 11))),
    _tempo(_config.get_random_tempo()),
    _signature(_config.get_signatures().get_random_item()) {
  do {
    _size = uint32_t(40 * Rand::next_normal()) + 26;
  } while(_size > 512); // <=512 with 46 average bars

  _size -= _size % 4; // Multiple of 4 bars
  _size *= get_bar_ticks();

  { // Generate chord progression
    _chord_progression = _config.get_chord_progression(_scale);
    for(uintptr_t i(0); i < bars(); i++) {
      for(uintptr_t j(0); j < get_bar_ticks(); j++) {
        _tones.push_back(_chord_progression[i % _chord_progression.size()].tones);
      }
    }
    assert(_tones.size() == _size);
  }

  { // Generate beats
    _beats.resize(get_bar_ticks());
    for(uint32_t i = 0; i < _beats.size(); i++) {
      _beats[i] = false;
      for(NoteValue j = get_beat_value(); j >= NoteValue(0); j = NoteValue(uint32_t(j) - 1)) {
        const auto ticks = ticks_for(j);
        if((i / ticks) * ticks == i) {
          if(Rand::next(uint32_t(0), (uint32_t(get_beat_value()) - uint32_t(j)) * 2) == 0) {
            _beats[i] = true;
            break;
          }
        }
      }
    }
  }

  for(const auto& creator : _config.get_creators()) {
    add_creator(creator->func(this));
  }

#ifndef NDEBUG
  check();
#endif
}
void Music::add_creator(Creator* creator) {
  creator->init();
  creator->post_init();
  paste(creator->compose(), _notes);
  _creators.push_back(std::unique_ptr<Creator>(creator));
}
const Chord& Music::chord_at(size_t i) const {
  return _chord_progression[(i / get_bar_ticks()) % _chord_progression.size()];
}
ToneSet Music::tones_at(size_t start, size_t size) const {
  ToneSet tones = ~0;
  const uintptr_t start_bar = start / get_bar_ticks();
  const uintptr_t end_bar = (start + size - 1) / get_bar_ticks();
  for(uintptr_t i = start_bar; i <= end_bar; i++) {
    tones &= _chord_progression[i % _chord_progression.size()].tones;
  }
  return tones;
}
bool Music::is_beat(uintptr_t i) const {
  return _beats[i % _beats.size()];
}
std::vector<uintptr_t> Music::beats_inside(uintptr_t min, uintptr_t max) const {
  std::vector<uintptr_t> beats;
  for(uintptr_t i = min; i <= max; i++) {
    if(is_beat(i)) {
      beats.push_back(i);
    }
  }
  return beats;
}
std::string Music::to_short_string() const {
  std::string short_string;
  short_string += get_scale().desc->name + "_" + key_name(get_scale().key);
  short_string += "_" + std::to_string(_signature->beats_per_bar);
  short_string += std::to_string(1 << (uint32_t(NoteValue::whole) - uint32_t(get_beat_value())));
  short_string += "_" + std::to_string(get_tempo());

  std::replace(short_string.begin(), short_string.end(), ' ', '_');

  return short_string;
}
void Music::check() const {
  Tones final_tones(octave_tones(_notes));
  assert(final_tones.size() <= _tones.size());
  for(uintptr_t i(0); i < final_tones.size(); i++) {
    assert(tone_set_within(_scale.tones, final_tones[i]));
    assert(tone_set_within(_tones[i], final_tones[i]));
  }

  for(const auto& note : _notes) {
    assert(is_beat(note.first));
  }
}
