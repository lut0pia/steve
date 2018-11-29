#pragma once

#include <cstdint>
#include <set>
#include <string>
#include <vector>

#include "Rand.h"
#include "Steve.h"

namespace steve {
  class Instrument {
  protected:
    static std::vector<Instrument> _instruments;
    std::string _name;
    uint8_t _midi_id, _min_tone, _max_tone;

  public:
    inline Instrument(uint8_t id, const char* name, uint8_t min_tone = 0, uint8_t max_tone = 60)
      : _name(name), _midi_id(id), _min_tone(std::max<uint8_t>(min_tone, 36)), _max_tone(std::min<uint8_t>(max_tone, 84)) {
    }

    // Return all tones corresponding to tones in the instrument's range
    std::set<uint8_t> in_range(const ToneSet& tones) const;

    inline const std::string& name() const { return _name; }
    inline const uint8_t& midi_id() const { return _midi_id; }
    inline const uint8_t& min_tone() const { return _min_tone; }
    inline const uint8_t& max_tone() const { return _max_tone; }
    inline uint8_t random_octave() const { return Rand::next(_min_tone/12, _max_tone/12)*12; }
    inline static void add(const Instrument& i) { _instruments.push_back(i); }
    inline static const Instrument* random() { return &Rand::in(_instruments); }
  };
}
