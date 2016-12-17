#include "Melody.h"

using namespace Steve;

Notes Melody::get(size_t start,size_t size,const std::vector<std::set<uint8_t>>& base) const {
  Notes notes;
  size_t i(0);
  uint8_t lasttone(-1);
  while(i<size) {
    size_t d(time(i,size));
    std::set<uint8_t> tones(possibilities(base,start+i,d,lasttone));
    while(tones.empty()) {
      if(!(d /= 2)) break;
      tones = possibilities(base,start+i,d,lasttone);
    }
    if(d>=pow(2,_mintime)) {
      uint8_t tone(Rand::in(tones));
      lasttone = tone;
      addNote(notes,_channel,tone,i,d);
      i += d;
    } else i++;
  }
  return notes;
}
std::set<uint8_t> Melody::possibilities(const std::vector<std::set<uint8_t>>& tones,size_t start,size_t size,uint8_t lasttone) const {
  return nearEnough(_instrument.inRange(Chord::possibilities(_scale,tonesAt(tones,start,size))),lasttone);
}
std::set<uint8_t> Melody::nearEnough(const std::set<uint8_t>& tones,uint8_t lasttone) const {
  if(lasttone==(uint8_t)-1)
    return tones;
  else {
    std::set<uint8_t> wtr;
    for(auto&& tone : tones)
      //if(abs((int)*tone-(int)lasttone)<=maxinter)
      if(!Rand::next(0,abs((int)tone-(int)lasttone)))
        wtr.insert(tone);
    return wtr;
  }
}
