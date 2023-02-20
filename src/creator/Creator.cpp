#include "Creator.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include "../Config.h"
#include "../Music.h"
#include "../Rand.h"

using namespace steve;

Creator::Creator(Music* music) {
  _music = music;
  _channel = music->creators().size();
}
void Creator::init() {
  _phrase_size = 4 * bar_ticks;

  do {
    _instrument = _music->get_config().get_random_instrument();
  } while(!is_valid_instrument(*_instrument));
  const uint8_t ambitus_half_range(Rand::next(6, 10));
  const uint8_t instrument_range(_instrument->max_tone - _instrument->min_tone);

  {
    const uint8_t mid_tone(_instrument->min_tone + ambitus_half_range + Rand::next_normal()*float(instrument_range - ambitus_half_range * 2));
    _min_tone = std::max<uint8_t>(mid_tone - ambitus_half_range, _instrument->min_tone);
    _max_tone = std::min<uint8_t>(mid_tone + ambitus_half_range, _instrument->max_tone);
    assert(_max_tone - _min_tone >= 12);
  }

  _min_time = Rand::next(NoteValue::sixteenth, NoteValue::quarter);
  _max_time = Rand::next(std::max(_min_time, NoteValue::quarter), NoteValue::whole);
  _repetition = Rand::next_float() * 0.5f;
}
Notes Creator::compose() {
  uint32_t i(0);
  Notes notes;
  while(i < _music->size()) {
    bool new_phrase_needed(true); // If it isn't changed it means no fitting phrase was found
    for(uintptr_t phrase_index(0); phrase_index < _phrases.size(); phrase_index++) { // Iterate through figures already created
      const Phrase& phrase(_phrases[phrase_index]);
      if(i%phrase.size == 0 // Its size is good for the place it would be in
        && i + phrase.size <= _music->size() // The phrase isn't too long
        && Rand::next_float() < _repetition // Add some randomness
        && harmony(_music->tones().data() + i, phrase.tones.data(), phrase.tones.size())) { // It's in harmony with the current music
        paste(phrase.notes, notes, i); // Paste the phrase on the music
        _phrase_list.push_back(phrase_index); // Register that we used this phrase
        i += phrase.size; // Go forth
        new_phrase_needed = false; // Specify that the program doesn't need to create a new phrase
        break;
      }
    }
    if(new_phrase_needed) { // Needs to create a new phrase of music
      Phrase phrase;
      phrase.size = _phrase_size;
      while(i%phrase.size != 0 || i + phrase.size > _music->size()) // Good size and not too long
        phrase.size /= 2;
      phrase.notes = get(i, phrase.size); // Create the phrase
      phrase.tones = octave_tones(phrase.notes);
      _phrase_list.push_back(_phrases.size()); // Register that we used this phrase
      _phrases.push_back(phrase); // Add it to the collection of phrases
      paste(phrase.notes, notes, i); // Paste it on the music
      i += phrase.size; // Go forth
    }
  }
  return notes;
}
bool Creator::is_valid_instrument(const Instrument&) const {
  return true;
}
void Creator::write_txt(std::ostream& s) const {
  s << "\t" << name() << " (" << _instrument->name << ")" << std::endl
    << "\t\t[" << note_value_name(_min_time) << ":" << note_value_name(_max_time) << "]" << std::endl
    << "\t\t[" << note_name(_min_tone) << ":" << note_name(_max_tone) << "]" << std::endl
    << "\t\tRepetition factor: " << _repetition << std::endl;

  {
    s << "\t\tPhrases:";
    for(uintptr_t phrase_index : _phrase_list) {
      s << " ";
      if(phrase_index < 10) {
        s << phrase_index;
      } else {
        s << char('a' + phrase_index - 10);
      }
      const Phrase& phrase(_phrases[phrase_index]);
      const uint32_t bar_count(phrase.size / bar_ticks);
      for(uint32_t i(1); i < bar_count; i++) {
        s << " -";
      }
    }
    s << std::endl;
  }
}
uintptr_t Creator::time(uintptr_t i, size_t size) const {
  std::vector<uintptr_t> candidates = _music->beats_inside(
    i + ticks_for(_min_time),
    i + std::min<uintptr_t>(ticks_for(_max_time), size));

  // Transform from position to duration
  for(uintptr_t& candidate : candidates) {
    candidate -= i;
  }

  const auto min_ticks = ticks_for(_min_time);
  const auto max_ticks = ticks_for(_max_time);
  const auto score = [this, i, min_ticks, max_ticks](uintptr_t c) -> uintptr_t {
    bool valid = false;
    for(auto ticks = min_ticks; ticks <= max_ticks; ticks <<= 1) {
      if(c == ticks || (ticks > std::max(min_ticks, ticks_for(NoteValue::eighth)) && ticks < max_ticks && c == (ticks * 3 / 2))) {
        valid = true;
        break;
      }
    }

    if(!valid) {
      return 0;
    }

    if(_music->tones_at(i, c) == 0) {
      return 0;
    }

    const bool is_pot = (c & (c - 1)) == 0;
    uintptr_t d = is_pot ? c : (c / 3);
    for(uintptr_t j = 2; d && j; d >>= 1, j--) {
      if(i % d == 0) {
        return bar_ticks * (j + (is_pot ? 1 : 0)) + c;
      }
    }
    return 0;
  };

  // Remove unwanted candidates
  auto previous_candidates = candidates;
  candidates.erase(std::remove_if(candidates.begin(), candidates.end(),
    [score](uintptr_t c) {
      return score(c) == 0;
    }), candidates.end());
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
std::vector<uintptr_t> Creator::generate_times(uintptr_t start, size_t size) const {
  std::vector<uintptr_t> times;
  do {
    uintptr_t i = 0;
    times = {i};
    while(i < size) {
      const uintptr_t d = time(start + i, size - i);
      if(d == 0) {
        break;
      }
      i += d;
      times.push_back(i);
    }
  } while(times.back() != size);
  return times;
}