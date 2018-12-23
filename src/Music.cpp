#include "Music.h"

#include <iostream>
#include <algorithm>
#include <cassert>

#include "Chord.h"
#include "creator.h"
#include "Scale.h"

using namespace steve;

Music::Music() : _scale(Scale::random()), _tempo(240*Rand::gauss(5)) {
  do _size = Rand::next(20)+26;
  while(_size>512); // <=512 with 46 average bars
  _size *= bar_ticks;

  { // Generate chord progression
    std::vector<ToneSet> chords(Chord::chords_in_harmony(_scale.tones(), 0));
    for(uintptr_t i(0); i<bars(); i++) {
      ToneSet chord(Rand::in(chords));
      for(uintptr_t j(0); j<bar_ticks; j++) {
        _tones.push_back(chord);
      }
    }
    assert(_tones.size()==_size);
  }

  do {
    Creator* creator;
    switch(Rand::next(0, 2)) {
      case 0: creator = new Melody(this); break;
      case 1: creator = new Chords(this); break;
      case 2: creator = new Arpeggio(this); break;
    }
    paste(creator->compose(), _notes);
    _creators.push_back(creator);
  } while((parts()<2 || Rand::next_float()>0.75f) && _creators.size()<9);
  if(Rand::next(0, 2)) {
    Creator* creator = new Drums(this);
    paste(creator->compose(), _notes);
    _creators.push_back(creator);
  }

  check();
}
ToneSet Music::tones_at(size_t start, size_t size) const {
  ToneSet wtr(_tones[start]);
  const uint32_t end(std::min(start+size, _tones.size()));
  for(uint32_t i(start+1); i<end; i++)
    wtr &= _tones[i];
  return wtr;
}
std::string Music::to_short_string() const {
  std::string short_string;
  short_string += scale().name() + "_";
  short_string += std::to_string(tempo());

  std::replace(short_string.begin(), short_string.end(), ' ', '_');

  return short_string;
}
void Music::check() const {
  Tones final_tones(octave_tones(_notes));
  for(ToneSet tones : final_tones) {
    assert(Chord::harmony(tones));
    assert((tones|_scale.tones()) == _scale.tones());
  }
}

static void write_bigendian(std::ostream& s, uint32_t v, uint32_t byteCount) {
  if(byteCount>=4) s << uint8_t(v>>24);
  if(byteCount>=3) s << uint8_t(v>>16);
  if(byteCount>=2) s << uint8_t(v>>8);
  if(byteCount>=1) s << uint8_t(v);
}
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
  for(uint32_t i(0); i<_creators.size(); i++) {
    s << uint8_t(0) << uint8_t(0xC0|i) << _creators[i]->instrument()->midi_id(); // Program change
  }
  uint32_t last(0);
  for(auto&& note : _notes) {
    s << uint8_t(note.first-last); // Track event delta time
    s << uint8_t((note.second.stop ? 0x80 : 0x90)|note.second.channel);
    s << uint8_t(note.second.tone);
    s << uint8_t(note.second.velocity);
    last = note.first;
  }
  s << uint8_t(0) << uint8_t(0xFF) << uint8_t(0x2F) << uint8_t(0); // End of track
  // Write chunk size
  const size_t endoff(s.tellp());
  s.seekp(sizeoff);
  write_bigendian(s, endoff-sizeoff-4, 4);
}
void Music::write_txt(std::ostream& s) const {
  s << "Scale: " << scale().name() << std::endl
    << "Tempo: " << tempo() << std::endl
    << "Duration: " << duration() << std::endl << std::endl;
  s << "Creators:" << std::endl;
  for(const Creator* creator : _creators) {
    creator->write_txt(s);
    s << std::endl;
  }
}