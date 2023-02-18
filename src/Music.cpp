#include "Music.h"

#include <iostream>
#include <algorithm>
#include <cassert>

#include "Chord.h"
#include "Config.h"
#include "Rand.h"
#include "Scale.h"

using namespace steve;

Music::Music(const Config& config)
  : _config(config),
    _scale(_config.get_random_scale()),
    _tempo(_config.get_random_tempo()) {
  do {
    _size = uint32_t(40 * Rand::next_normal()) + 26;
  } while(_size > 512); // <=512 with 46 average bars
  
  _size -= _size % 4; // Multiple of 4 bars
  _size *= bar_ticks;

  { // Generate chord progression
    _chord_progression = _config.get_chord_progression(_scale);
    for(uintptr_t i(0); i<bars(); i++) {
      for(uintptr_t j(0); j<bar_ticks; j++) {
        _tones.push_back(_chord_progression[i % _chord_progression.size()].tones);
      }
    }
    assert(_tones.size()==_size);
  }

  { // Generate beats
    
    for(uint32_t i = 0; i < 4; i++) {
      const uint32_t offset = ticks_for(NoteValue::quarter) * i;
      _beats.push_back(offset + 0);
      switch(Rand::next(0, 7)) {
        case 0: break; // 4
        case 1: // 22
          _beats.push_back(offset + ticks_for(NoteValue::eighth));
          break;
        case 2: // 1111
          _beats.push_back(offset + ticks_for(NoteValue::sixteenth));
          _beats.push_back(offset + ticks_for(NoteValue::sixteenth) * 2);
          _beats.push_back(offset + ticks_for(NoteValue::sixteenth) * 3);
          break;
        case 3: // 211
          _beats.push_back(offset + ticks_for(NoteValue::eighth));
          _beats.push_back(offset + ticks_for(NoteValue::sixteenth) * 3);
          break;
        case 4: // 112
          _beats.push_back(offset + ticks_for(NoteValue::sixteenth));
          _beats.push_back(offset + ticks_for(NoteValue::sixteenth) * 2);
          break;
        case 5: // 31
          _beats.push_back(offset + ticks_for(NoteValue::sixteenth) * 3);
          break;
        case 6: // 13
          _beats.push_back(offset + ticks_for(NoteValue::sixteenth));
          break;
        case 7: // 121
          _beats.push_back(offset + ticks_for(NoteValue::sixteenth));
          _beats.push_back(offset + ticks_for(NoteValue::sixteenth) * 3);
          break;
      }
    }
    
    _beat_mod = bar_ticks;
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
  paste(creator->compose(), _notes);
  _creators.push_back(std::unique_ptr<Creator>(creator));
}
const Chord& Music::chord_at(size_t i) const {
  return _chord_progression[(i / bar_ticks) % _chord_progression.size()];
}
ToneSet Music::tones_at(size_t start, size_t size) const {
  ToneSet tones = ~0;
  const uintptr_t start_bar = start / bar_ticks;
  const uintptr_t end_bar = (start + size - 1) / bar_ticks;
  for(uintptr_t i = start_bar; i <= end_bar; i++) {
      tones &= _chord_progression[i % _chord_progression.size()].tones;
  }
  return tones;
}
bool Music::is_beat(uintptr_t i) const {
  i %= _beat_mod;
  return std::find(_beats.begin(), _beats.end(), i) != _beats.end();
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
  short_string += scale().desc->name + "_" + key_name(scale().key) + "_";
  short_string += std::to_string(tempo());

  std::replace(short_string.begin(), short_string.end(), ' ', '_');

  return short_string;
}
void Music::check() const {
  Tones final_tones(octave_tones(_notes));
  assert(final_tones.size() <= _tones.size());
  for(uintptr_t i(0); i < final_tones.size(); i++) {
    assert((final_tones[i] | _scale.tones) == _scale.tones);
    assert((final_tones[i] | _tones[i]) == _tones[i]);
  }

  for(const auto& note : _notes) {
    assert(is_beat(note.first));
  }
}

static void write_bigendian(std::ostream& s, uint32_t v, uint32_t byteCount) {
  if(byteCount>=4) s << uint8_t(v>>24);
  if(byteCount>=3) s << uint8_t(v>>16);
  if(byteCount>=2) s << uint8_t(v>>8);
  if(byteCount>=1) s << uint8_t(v);
}
class VarLength {
  uint32_t _value;
public:
  inline VarLength(uint32_t value) : _value(value) {}
  friend inline std::ostream& operator<<(std::ostream& s, const VarLength& v) {
    if(v._value >= 128) {
      s << (uint8_t)(((v._value >> 7) & 0x7f) | 0x80);
    }
    s << (uint8_t)(v._value & 0x7f);
    return s;
  }
};
void Music::write_mid(std::ostream& s) const {
  // Header chunk
  s << "MThd"; // Chunk id
  write_bigendian(s, 6, 4); // Chunk size
  write_bigendian(s, 0, 2); // Format type (single track)
  write_bigendian(s, 1, 2); // Number of tracks
  write_bigendian(s, ticks_for(NoteValue::quarter), 2); // Time division (ticks per beat)

  // Track chunk
  s << "MTrk";
  const size_t sizeoff(s.tellp());
  write_bigendian(s, 0, 4); // Placeholder for track size
  s << uint8_t(0); // Track event delta time
  s << uint8_t(0xFF) << uint8_t(0x51) << uint8_t(3); // Tempo meta event
  write_bigendian(s, 60000000u/_tempo, 3); // Microseconds per quarter note
  for(uint32_t i(0); i < _creators.size(); i++) {
    s << uint8_t(0) << uint8_t(0xC0|i) << _creators[i]->instrument()->midi_id; // Program change
  }

  uint32_t last = 0;
  uint32_t last_chord = -1;

  Notes end_notes;
  auto process_end_notes = [&s, &last, &end_notes](uint32_t next_time) {
    while(!end_notes.empty()) {
      auto next_end = end_notes.begin();
      if(next_end->first <= next_time) {
        s << VarLength(next_end->first - last) << uint8_t(0x80 | next_end->second.channel) << uint8_t(next_end->second.tone) << uint8_t(next_end->second.velocity); // Note off
        last = next_end->first;
        end_notes.erase(next_end);
      } else {
        break;
      }
    }
  };

  for(const auto& note : _notes) {
    process_end_notes(note.first);

    s << VarLength(note.first - last) << uint8_t(0x90 | note.second.channel) << uint8_t(note.second.tone) << uint8_t(note.second.velocity); // Note on

    end_notes.insert(std::make_pair(note.first + note.second.duration, note.second));

    last = note.first;

    if(note.first != last_chord && note.first != _size && note.first % bar_ticks == 0) {
      // Chord meta-event
      const Chord chord = chord_at(note.first);
      const uint8_t degree =_scale.get_degree_for_tone(chord.key);
      const std::string chord_str = chord_at(note.first).to_short_string() + " (" + std::to_string(degree + 1) + ")";
      s << uint8_t(0) << uint8_t(0xFF) << uint8_t(0x01) << VarLength(chord_str.size()) << chord_str;
      last_chord = note.first;
    }
  }
  process_end_notes(last + bar_ticks);

  s << uint8_t(0) << uint8_t(0xFF) << uint8_t(0x2F) << uint8_t(0); // End of track

  // Write chunk size
  const size_t endoff(s.tellp());
  s.seekp(sizeoff);
  write_bigendian(s, endoff-sizeoff-4, 4);
}
void Music::write_txt(std::ostream& s) const {
  s << "Scale: " << key_name(scale().key) << " " << scale().desc->name << std::endl
    << "Tempo: " << tempo() << std::endl
    << "Duration: " << duration() << std::endl << std::endl;

  {
    s << "Chord progression:" << std::endl;
    for(const Chord& chord : _chord_progression) {
      s << " - " << chord.to_short_string() << std::endl;
    }
    s << std::endl;
  }

  {
    s << "Rhythm:" << std::endl << '\t';
    for(uintptr_t i = 0; i < _beat_mod; i += 2) {
      if(i % ticks_for(NoteValue::quarter) == 0 && i > 0) {
        s << ' ';
      }
      s << (is_beat(i) ? '1' : '0');
    }
    s << std::endl << std::endl;
  }

  s << "Creators:" << std::endl;
  for(const auto& creator : _creators) {
    creator->write_txt(s);
    s << std::endl;
  }
}