#include "Instrument.h"

using namespace steve;

std::vector<Instrument> Instrument::_instruments;

std::set<uint8_t> Instrument::in_range(const std::set<uint8_t>& tones) const {
  std::set<uint8_t> wtr;
  for(auto&& tone : tones)
    for(uint8_t t(tone); t<=_max_tone; t += 12)
      if(t>=_min_tone)
        wtr.insert(t);
  return wtr;
}
