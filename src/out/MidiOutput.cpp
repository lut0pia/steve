#include "MidiOutput.h"

#include <iostream>

#include "../ctr/Creator.h"
#include "../cfg/Instrument.h"

using namespace steve;

const uint8_t mid_meta_event = 0xff;
const uint8_t mid_text_event = 0x01;
const uint8_t mid_copyright_notice = 0x02;
const uint8_t mid_instrument_name = 0x04;
const uint8_t mid_time_signature1 = 0x58;
const uint8_t mid_time_signature2 = 0x04;

static void write_bigendian(std::ostream& s, uint32_t v, uint32_t byteCount) {
  if(byteCount >= 4) {
    s << uint8_t(v >> 24);
  }
  if(byteCount >= 3) {
    s << uint8_t(v >> 16);
  }
  if(byteCount >= 2) {
    s << uint8_t(v >> 8);
  }
  if(byteCount >= 1) {
    s << uint8_t(v);
  }
}

class VarLength {
  uint32_t _value;

public:
  inline VarLength(uint32_t value)
    : _value(value) {}
  friend inline std::ostream& operator<<(std::ostream& s, const VarLength& v) {
    if(v._value >= 128) {
      s << (uint8_t)(((v._value >> 7) & 0x7f) | 0x80);
    }
    s << (uint8_t)(v._value & 0x7f);
    return s;
  }
};

void steve::MidiOutput::write(const Music& music, std::ostream& s) {
  // Header chunk
  s << "MThd"; // Chunk id
  write_bigendian(s, 2 * 3, 4); // Chunk size (the 3 next 16bit integers)
  write_bigendian(s, 0, 2); // Format type (single track)
  write_bigendian(s, 1, 2); // Number of tracks
  write_bigendian(s, ticks_for(NoteValue::quarter), 2); // Time division (ticks per beat)

  // Track chunk
  s << "MTrk";
  const size_t sizeoff(s.tellp());
  write_bigendian(s, 0, 4); // Placeholder for track size

  { // Tempo meta event
    s << uint8_t(0) << uint8_t(0xff) << uint8_t(0x51) << uint8_t(3);
    write_bigendian(s, 60000000u / music.get_tempo(), 3); // Microseconds per quarter note
  }

  { // Time signature meta event
    s << uint8_t(0) << mid_meta_event << mid_time_signature1 << mid_time_signature2
      << uint8_t(music.get_signature().beats_per_bar) // Numerator
      << uint8_t(uint8_t(NoteValue::whole) - uint8_t(music.get_beat_value())) // Denominator (2^x)
      << uint8_t(0x18) // Metronome pulse in clock ticks
      << uint8_t(ticks_for(music.get_beat_value()) / ticks_for(NoteValue::thirtysecond)); // 32nd per beat
  }

  const auto& creators = music.get_creators();
  for(uint32_t i(0); i < creators.size(); i++) {
    s << uint8_t(0) << uint8_t(0xC0 | i) << creators[i]->instrument()->midi_id; // Program change
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

  for(const auto& note : music.get_notes()) {
    process_end_notes(note.first);

    s << VarLength(note.first - last) << uint8_t(0x90 | note.second.channel) << uint8_t(note.second.tone) << uint8_t(note.second.velocity); // Note on

    end_notes.insert(std::make_pair(note.first + note.second.duration, note.second));

    last = note.first;

    if(note.first != last_chord && note.first != music.get_size() && note.first % music.get_bar_ticks() == 0) {
      // Chord meta-event
      const Chord chord = music.chord_at(note.first);
      const uint8_t degree = music.get_scale().get_degree_for_tone(chord.key);
      const std::string chord_str = music.chord_at(note.first).to_short_string() + " (" + std::to_string(degree + 1) + ")";
      s << uint8_t(0) << mid_meta_event << mid_text_event << VarLength(uint32_t(chord_str.size())) << chord_str;
      last_chord = note.first;
    }
  }
  process_end_notes(last + music.get_bar_ticks());

  s << uint8_t(0) << mid_meta_event << uint8_t(0x2F) << uint8_t(0); // End of track

  // Write chunk size
  const size_t endoff(s.tellp());
  s.seekp(sizeoff);
  write_bigendian(s, endoff - sizeoff - 4, 4);
}