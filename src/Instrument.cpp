#include "Instrument.h"

using namespace steve;

std::vector<Instrument> Instrument::_instruments;

std::set<uint8_t> Instrument::in_range(const ToneSet& tones) const {
  std::set<uint8_t> wtr;
  for(uint8_t tone(0); tone<12; tone++) {
    if(tones & (1<<tone)) {
      for(uint8_t t(tone); t<=_max_tone; t += 12)
        if(t>=_min_tone)
          wtr.insert(t);
    }
  }
  return wtr;
}
