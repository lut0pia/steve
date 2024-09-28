#include "TextOutput.h"

#include <iostream>

#include "../cfg/Instrument.h"
#include "../ctr/Creator.h"

using namespace steve;

void steve::TextOutput::write(const Music& music, std::ostream& s) {
  s << "Scale: " << key_name(music.get_scale().key) << " " << music.get_scale().desc->name << std::endl
    << "Tempo: " << music.get_tempo() << std::endl
    << "Signature: " << music.get_signature().beats_per_bar << "/" << music.get_signature().beat_value_numeral << std::endl
    << "Duration: " << music.get_duration() << std::endl
    << std::endl;

  {
    s << "Progression:" << std::endl;
    for(const Chord& chord : music.get_progression()) {
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
    s << "\t" << creator->name() << " (" << creator->get_instrument()->name << ")" << std::endl
      << "\t\t[" << note_value_name(creator->get_min_time()) << ":" << note_value_name(creator->get_max_time()) << "]" << std::endl
      << "\t\t[" << note_name(creator->get_min_tone()) << ":" << note_name(creator->get_max_tone()) << "]" << std::endl
      << "\t\tRepetition factor: " << creator->get_repetition() << std::endl;

    {
      s << "\t\Patterns:";
      for(uintptr_t pattern_index : creator->get_pattern_list()) {
        s << " ";
        if(pattern_index < 10) {
          s << pattern_index;
        } else {
          s << char('a' + pattern_index - 10);
        }
        const Pattern& pattern(creator->get_patterns()[pattern_index]);
        const uint32_t bar_count(pattern.size / music.get_bar_ticks());
        for(uint32_t i(1); i < bar_count; i++) {
          s << " -";
        }
      }
      s << std::endl;
    }
    s << std::endl;
  }
}