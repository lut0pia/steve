#include "Rand.h"

#include <ctime>

using namespace steve;

const uint64_t a(2862933555777941757LL), b(3037000493LL);
uint64_t Rand::_state(time(nullptr));

uint64_t Rand::next() {
  return _state = _state*a+b;
}
int Rand::next_int() {
  return next()>>32;
}
uint8_t Rand::next_byte() {
  return next()>>48;
}
float Rand::next_float() {
  return next()/(float)(uint64_t)-1;
}

uint64_t Rand::next(uint64_t min, uint64_t max) {
  return (next()%(max-min+1))+min;
}
int Rand::next(int min, int max) {
  return ((next()>>32)%(max-min+1))+min;
}
float Rand::next(float min, float max) {
  return (next_float()*(max-min))+min;
}

float Rand::gauss(uint32_t i) {
  float wtr(0);
  for(uint32_t j(0); j<i; j++)
    wtr += next_float();
  return wtr/i;
}
uint64_t Rand::next(uint64_t ave) {
  if(ave) {
    uint64_t wtr(0), thres(((uint64_t)-1)/(ave+1));
    while(next()>thres) wtr++;
    return wtr;
  } else return 0;
}
