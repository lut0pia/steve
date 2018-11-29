#include "Scale.h"

using namespace steve;

std::vector<Scale> Scale::scales;

Scale::Scale(const char* name, std::initializer_list<uint8_t> tones) :
  _name(name), _key(0), _tones(0) {
  for(uint8_t tone : tones) {
    _tones |= 1<<tone;
  }
}