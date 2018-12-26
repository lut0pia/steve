#include "Melody.h"

using namespace steve;

Melody::Melody(Music* music) : Creator(music) {
  _maxinter = 3;
}
Notes Melody::get(size_t start, size_t size) const {
  Notes notes;
  size_t i(0);
  uint8_t last_tone(-1);
  while(i<size) {
    size_t d(time(i, size));
    std::set<uint8_t> tones(choose_note_from_chord(_music->tones_at(start+i, d), last_tone));
    while(tones.empty()) {
      if(!(d /= 2)) break;
      tones = choose_note_from_chord(_music->tones_at(start+i, d), last_tone);
    }
    if(d >= (1<<_min_time)) {
      uint8_t tone(Rand::in(tones));
      last_tone = tone;
      add_note(notes, _channel, tone, i, d);
      i += d;
    } else i++;
  }
  return notes;
}
std::set<uint8_t> Melody::choose_note_from_chord(const ToneSet& tones, uint8_t lasttone) const {
  return near_enough(_instrument->in_range(tones), lasttone);
}
std::set<uint8_t> Melody::near_enough(const std::set<uint8_t>& tones, uint8_t last_tone) const {
  if(last_tone==(uint8_t)-1)
    return tones;
  else {
    auto beg(tones.find(last_tone));
    auto end(beg);
    for(uint32_t i(0); i < 2 && beg != tones.begin(); i++) {
      --beg;
    }
    for(uint32_t i(0); i < 2 && end != tones.end(); i++) {
      ++end;
    }
      
    return std::set<uint8_t>(beg, end);
  }
}
