#include "Creator.h"

#include <algorithm>

#include "../Rand.h"

using namespace Steve;

size_t Creator::time(size_t i,size_t size) const {
  size_t wtr(pow(2,Rand::next(_mintime,_maxtime))); // 2^n ticks
  while(i%wtr!=0 || i+wtr>size) // Good size and not too long
    wtr /= 2;
  return wtr;
}
std::set<uint8_t> Creator::tonesAt(const std::vector<std::set<uint8_t>>& tones,size_t start,size_t size) const {
  std::set<uint8_t> wtr;
  for(uint32_t i(start); i<std::min(start+size,tones.size()); i++)
    wtr.insert(tones[i].begin(),tones[i].end());
  return wtr;
}
