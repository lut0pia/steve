#pragma once

#include "Rand.h"
#include "Steve.h"

namespace Steve{
  class Chord{
  protected:
    static std::vector<std::set<uint8_t>> _chords;

  public:
    static std::set<uint8_t> possibilities(const std::set<uint8_t>& scale,const std::set<uint8_t>& played);
    static std::vector<std::set<uint8_t>> in(const std::set<uint8_t>& scale,const std::set<uint8_t>& played);
    static bool harmony(const std::set<uint8_t>& played);
    inline static void add(const std::initializer_list<uint8_t>& il) { _chords.emplace_back(il); }
    inline static std::set<uint8_t> random() { return Rand::in(_chords); }
  };
}
