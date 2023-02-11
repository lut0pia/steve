#include "Chord.h"

using namespace steve;

std::string Chord::to_short_string() const {
  return key_name(key) + desc->suffix;
}
