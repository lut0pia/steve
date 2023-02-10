#include "Chord.h"

#include "Scale.h"

using namespace steve;

std::string Chord::to_short_string() const {
  return key_name(_key) + _desc->suffix;
}
