#include "TextOutput.h"

#include <iostream>

#include "../creator/Creator.h"

using namespace steve;

void steve::TextOutput::write(const Music& music, std::ostream& s) {
  s << "Scale: " << key_name(music.get_scale().key) << " " << music.get_scale().desc->name << std::endl
    << "Tempo: " << music.get_tempo() << std::endl
    << "Signature: " << music.get_signature().beats_per_bar << "/" << (1 << (uint32_t(NoteValue::whole) - uint32_t(music.get_beat_value()))) << std::endl
    << "Duration: " << music.duration() << std::endl
    << std::endl;

  {
    s << "Chord progression:" << std::endl;
    for(const Chord& chord : music.get_chord_progression()) {
      s << " - " << music.get_scale().get_degree_string_for_chord(chord) << " (" << chord.to_short_string() << ")" << std::endl;
    }
    s << std::endl;
  }

  {
    s << "Rhythm:" << std::endl
      << '\t';
    for(uintptr_t i = 0; i < music.get_beats().size(); i += 2) {
      if(i % ticks_for(music.get_beat_value()) == 0 && i > 0) {
        s << ' ';
      }
      s << (music.is_beat(i) ? '1' : '0');
    }
    s << std::endl
      << std::endl;
  }

  s << "Creators:" << std::endl;
  for(const auto& creator : music.get_creators()) {
    creator->write_txt(s);
    s << std::endl;
  }
}