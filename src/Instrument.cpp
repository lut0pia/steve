#include "Instrument.h"

using namespace Steve;

std::vector<Instrument> Instrument::_instruments;

std::set<uint8_t> Instrument::inRange(const std::set<uint8_t>& tones) const {
  std::set<uint8_t> wtr;
  for(auto&& tone : tones)
    for(uint8_t t(tone); t<=_maxTone; t += 12)
      if(t>=_minTone)
        wtr.insert(t);
  return wtr;
}
