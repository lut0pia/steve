#include "Creator.h"

#include <algorithm>
#include <cassert>
#include <iostream>

#include "../Music.h"
#include "../Rand.h"
#include "../cfg/Chord.h"
#include "../cfg/Config.h"

using namespace steve;

Creator::Creator(Music* music) {
  _music = music;
  _channel = music->get_creators().size();
}
void Creator::init() {
  _pattern_size = _music->get_progression().size() * _music->get_beat_ticks();

  _instrument = _music->get_config().get_instruments().get_random_item(
    [this](std::shared_ptr<Instrument> candidate) {
      return is_valid_instrument(*candidate);
    });
  const uint8_t ambitus_half_range(Rand::next(6, 10));
  const uint8_t instrument_range(_instrument->max_tone - _instrument->min_tone);

  {
    const uint8_t mid_tone(_instrument->min_tone + ambitus_half_range + Rand::next_normal() * float(instrument_range - ambitus_half_range * 2));
    _min_tone = std::max<uint8_t>(mid_tone - ambitus_half_range, _instrument->min_tone);
    _max_tone = std::min<uint8_t>(mid_tone + ambitus_half_range, _instrument->max_tone);
    assert(_max_tone - _min_tone >= 12);
  }

  _min_time = Rand::next(NoteValue::sixteenth, NoteValue::quarter);
  _max_time = Rand::next(std::max(_min_time, NoteValue::quarter), NoteValue::whole);
  _repetition = Rand::next_float() * 0.5f;
}
void Creator::post_init() {
  { // Make sure we can make notes the size of the beat
    // Otherwise we may not be able to complete a bar
    if(_min_time > _music->get_beat_value()) {
      _min_time = _music->get_beat_value();
    }
    if(_max_time < _music->get_beat_value()) {
      _max_time = _music->get_beat_value();
    }
  }
}

Notes Creator::compose() {
  uint32_t i(0);
  Notes notes;
  while(i < _music->get_tick_count()) {
    bool new_pattern_needed(true); // If it isn't changed it means no fitting pattern was found
    for(uintptr_t pattern_index(0); pattern_index < _patterns.size(); pattern_index++) { // Iterate through figures already created
      const Pattern& pattern(_patterns[pattern_index]);
      if(i % pattern.size == 0 // Its size is good for the place it would be in
         && i + pattern.size <= _music->get_tick_count() // The pattern isn't too long
         && Rand::next_float() < _repetition) { // Add some randomness
        paste(pattern.notes, notes, i); // Paste the pattern on the music
        _pattern_list.push_back(pattern_index); // Register that we used this pattern
        i += pattern.size; // Go forth
        new_pattern_needed = false; // Specify that the program doesn't need to create a new pattern
        break;
      }
    }
    if(new_pattern_needed) { // Needs to create a new pattern of music
      Pattern pattern;
      pattern.size = _pattern_size;
      while(i % pattern.size != 0 || i + pattern.size > _music->get_tick_count()) // Good size and not too long
        pattern.size /= 2;
      pattern.notes = get(i, pattern.size); // Create the pattern
      pattern.tones = octave_tones(pattern.notes);
      _pattern_list.push_back(_patterns.size()); // Register that we used this pattern
      _patterns.push_back(pattern); // Add it to the collection of patterns
      paste(pattern.notes, notes, i); // Paste it on the music
      i += pattern.size; // Go forth
    }
  }
  return notes;
}
bool Creator::is_valid_instrument(const Instrument&) const {
  return true;
}
void Creator::add_note(Notes& notes, uint8_t tone, size_t start, size_t length, uint8_t velocity) const {
  assert(_music->tones_at(start, length) & (1 << (tone % 12)));
  assert(_music->is_beat(start));
  steve::add_note(notes, _channel, tone, start, length, velocity);
}
uintptr_t Creator::generate_time(uintptr_t i, size_t size, bool chord_strict) const {
  std::vector<uintptr_t> candidates = _music->beats_inside(
    i + ticks_for(_min_time),
    i + std::min<uintptr_t>(ticks_for(_max_time), size));
  assert(!candidates.empty());

  // Transform from position to duration
  for(uintptr_t& candidate : candidates) {
    candidate -= i;
  }

  const auto min_ticks = ticks_for(_min_time);
  const auto max_ticks = ticks_for(_max_time);
  const auto score = [this, i, chord_strict, min_ticks, max_ticks](uintptr_t c) -> uintptr_t {
    if(c < min_ticks || c > max_ticks) {
      return 0;
    }

    if((c / min_ticks) * min_ticks != c) {
      return 0;
    }

    const ToneSet available_tones_over_duration = _music->tones_at(i, c);
    if(available_tones_over_duration == 0) {
      return 0; // There are no common tones over that period
    }

    if(chord_strict) {
      const ToneSet available_instant_tones = _music->tones_at(i, 1);
      if(available_instant_tones != available_tones_over_duration) {
        return 0; // Caller disallowed notes that overlap chord changes
      }
    }

    uintptr_t score = UINTPTR_MAX;
    for(uintptr_t i = 0; i < sizeof(uintptr_t) * 8; i++) {
      // Low set bits reduce score greatly
      // High set bits reduce score slightly
      if(((1ull << i) & c) != 0) {
        score -= 1ull << (sizeof(uintptr_t) * 8 - i - 1);
      }
    }
    return score;
  };

  // Remove unwanted candidates
  auto previous_candidates = candidates;
  candidates.erase(std::remove_if(candidates.begin(), candidates.end(), [score](uintptr_t c) {
    return score(c) == 0;
  }),
    candidates.end());
  if(candidates.empty()) {
    return 0;
  }

  // Sort by increasing awkwardness
  std::sort(candidates.begin(), candidates.end(),
    [score](uintptr_t a, uintptr_t b) {
      return score(a) > score(b);
    });

  std::exponential_distribution<float> dist(0.5f);
  const uintptr_t index = std::max<uintptr_t>(0, std::min<uintptr_t>(candidates.size() - 1, dist(Rand::generator)));

  const size_t ticks = candidates[index];
  assert(ticks <= size);
  return ticks;
}
std::vector<uintptr_t> Creator::generate_times(uintptr_t start, size_t size, bool chord_key_strict) const {
  std::vector<uintptr_t> times;
  do {
    uintptr_t i = 0;
    times = {i};
    while(i < size) {
      const uintptr_t d = generate_time(start + i, size - i, chord_key_strict);
      if(d == 0) {
        break;
      }
      i += d;
      times.push_back(i);
    }
  } while(times.back() != size);
  return times;
}