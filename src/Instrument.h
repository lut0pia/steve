#pragma once

#include <cstdint>
#include <set>
#include <string>
#include <vector>

#include "Rand.h"

namespace Steve {
  class Instrument {
  protected:
    std::string _name;
    uint8_t _midiId,_minTone,_maxTone;
    static std::vector<Instrument> _instruments;

  public:
    inline Instrument(uint8_t id,const char* name,uint8_t minTone = 0,uint8_t maxTone = 60)
      : _name(name),_midiId(id),_minTone(minTone),_maxTone(maxTone) {}
    inline const std::string& name() const { return _name; }
    inline const uint8_t& midiId() const { return _midiId; }
    inline const uint8_t& minTone() const { return _minTone; }
    inline const uint8_t& maxTone() const { return _maxTone; }
    inline uint8_t randomOctave() const { return Rand::next(_minTone/12,_maxTone/12)*12; }
    std::set<uint8_t> inRange(const std::set<uint8_t>& tones) const; // Returns all tones corresponding to tones in the instrument's range
    inline static void add(const Instrument& i) { _instruments.push_back(i); }
    inline static const Instrument* random() { return &Rand::in(_instruments); }
  };
}
