#include "Melody.h"

using namespace steve;

Notes Melody::get(size_t start, size_t size) const {
  Notes notes;
  size_t i(0);
  uint8_t last_tone(-1);
  while(i<size) {
    size_t d(time(i, size));
    std::set<uint8_t> tones(notes_in_harmony(_music.tones(), start+i, d, last_tone));
    while(tones.empty()) {
      if(!(d /= 2)) break;
      tones = notes_in_harmony(_music.tones(), start+i, d, last_tone);
    }
    if(d>=pow(2, _mintime)) {
      uint8_t tone(Rand::in(tones));
      last_tone = tone;
      add_note(notes, _channel, tone, i, d);
      i += d;
    } else i++;
  }
  return notes;
}
std::set<uint8_t> Melody::notes_in_harmony(const Tones& tones, size_t start, size_t size, uint8_t lasttone) const {
  return near_enough(_instrument.in_range(Chord::tones_in_harmony(_scale.tones(), _music.tones_at(start, size))), lasttone);
}
std::set<uint8_t> Melody::near_enough(const std::set<uint8_t>& tones, uint8_t last_tone) const {
  if(last_tone==(uint8_t)-1)
    return tones;
  else {
    std::set<uint8_t> wtr;
    for(auto&& tone : tones)
      //if(abs((int)*tone-(int)last_tone)<=maxinter)
      if(!Rand::next(0, abs((int)tone-(int)last_tone)))
        wtr.insert(tone);
    return wtr;
  }
}
