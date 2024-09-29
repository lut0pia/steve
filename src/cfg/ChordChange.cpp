#include "ChordChange.h"

using namespace steve;

void ChordChange::compute_cache() {
  if(source_chord->blacklisted || target_chord->blacklisted) {
    blacklisted = true;
  }
  if(source_chord->whitelisted && target_chord->whitelisted) {
    whitelisted = true;
  }
}
