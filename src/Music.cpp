#include "Music.h"

#include <iostream>
#include <algorithm>
#include <cassert>

#include "Chord.h"
#include "creator.h"
#include "Scale.h"

using namespace steve;

Music::Music() : _scale(Scale::random()), _tempo((uint32_t(240 * Rand::gauss(5)) / 5) * 5) {
  do _size = Rand::next(20)+26;
  while(_size>512); // <=512 with 46 average bars
  _size -= _size % 4; // Multiple of 4 bars
  _size *= bar_ticks;

  { // Generate chord progression
    std::vector<Chord> chords(Chord::chords_inside(_scale));
    uintptr_t period(4);
    for(uintptr_t i(0); i < period; i++) {
      _chord_progression.push_back(Rand::in(chords));
    }
    for(uintptr_t i(0); i<bars(); i++) {
      for(uintptr_t j(0); j<bar_ticks; j++) {
        _tones.push_back(_chord_progression[i % period].tones());
      }
    }
    assert(_tones.size()==_size);
  }

  add_creator(new Drums(this));
  add_creator(new Bass(this));

  do {
    add_creator(new Melody(this));
  } while((parts() < 3 || Rand::next_float() > 0.75f) && _creators.size() < 9);

  check();
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
      tones &= _chord_progression[i % _chord_progression.size()].tones();
  }
  return tones;
}
std::string Music::to_short_string() const {
  std::string short_string;
  short_string += scale().name() + "_" + key_name(scale().key()) + "_";
  short_string += std::to_string(tempo());

  std::replace(short_string.begin(), short_string.end(), ' ', '_');

  return short_string;
}
void Music::check() const {
  Tones final_tones(octave_tones(_notes));
  for(uintptr_t i(0); i < final_tones.size(); i++) {
    assert((final_tones[i] | _scale.tones()) == _scale.tones());
    assert((final_tones[i] | _tones[i]) == _tones[i]);
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
  write_bigendian(s, ticks_for(quarter), 2); // Time division (ticks per beat)
  // Track chunk
  s << "MTrk";
  const size_t sizeoff(s.tellp());
  write_bigendian(s, 0, 4); // Placeholder for track size
  s << uint8_t(0); // Track event delta time
  s << uint8_t(0xFF) << uint8_t(0x51) << uint8_t(3); // Tempo meta event
  write_bigendian(s, 60000000u/_tempo, 3); // Microseconds per quarter note
  for(uint32_t i(0); i < _creators.size(); i++) {
    s << uint8_t(0) << uint8_t(0xC0|i) << _creators[i]->instrument()->midi_id(); // Program change
  }
  uint32_t last = 0;
  uint32_t last_chord = -1;
  for(const auto& note : _notes) {
    s << VarLength(note.first - last); // Track event delta time
    s << uint8_t((note.second.stop ? 0x80 : 0x90)|note.second.channel);
    s << uint8_t(note.second.tone);
    s << uint8_t(note.second.velocity);
    last = note.first;
    if(note.first != last_chord && note.first != _size && note.first % bar_ticks == 0) {
      // Chord meta-event
      std::string chord_str = chord_at(note.first).to_short_string();
      s << uint8_t(0) << uint8_t(0xFF) << uint8_t(0x01) << VarLength(chord_str.size()) << chord_str; // Program change
      last_chord = note.first;
    }
  }
  s << uint8_t(0) << uint8_t(0xFF) << uint8_t(0x2F) << uint8_t(0); // End of track
  // Write chunk size
  const size_t endoff(s.tellp());
  s.seekp(sizeoff);
  write_bigendian(s, endoff-sizeoff-4, 4);
}
void Music::write_txt(std::ostream& s) const {
  s << "Scale: " << scale().full_name() << std::endl
    << "Tempo: " << tempo() << std::endl
    << "Duration: " << duration() << std::endl << std::endl;

  {
    s << "Chord progression:" << std::endl;
    for(const Chord& chord : _chord_progression) {
      s << " - " << chord.to_short_string() << std::endl;
    }
    s << std::endl;
  }

  s << "Creators:" << std::endl;
  for(const auto& creator : _creators) {
    creator->write_txt(s);
    s << std::endl;
  }
}