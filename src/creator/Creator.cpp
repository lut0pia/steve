#include "Creator.h"

#include "../Rand.h"

using namespace steve;

void Creator::write_txt(std::ostream& s) const {
  s << "\t" << name() << " (" << _instrument.name() << ")" << std::endl
    << "\t[" << note_value_name(_mintime) << ":" << note_value_name(_maxtime) << "]" << std::endl
    << std::endl;
}
size_t Creator::time(size_t i, size_t size) const {
  size_t wtr(1 << Rand::next(_mintime, _maxtime)); // 2^n ticks
  while(i%wtr != 0 || i + wtr > size) // Good size and not too long
    wtr /= 2;
  return wtr;
}
