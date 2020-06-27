#include "Scale.h"

using namespace steve;

std::vector<Scale::Description> Scale::_descriptions;

Scale::Description::Description(const char* name, std::initializer_list<uint8_t> tone_list)
  : name(name), tones(0) {
  for(uint8_t tone : tone_list) {
    tones |= 1 << tone;
  }
}