#include "Creator.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include "../Music.h"
#include "../Rand.h"

using namespace steve;

Creator::Creator(Music* music) {
  _music = music;
  _channel = music->creators().size();
}
void Creator::init() {
  _figure_size = (1ull << Rand::next(0ull, 3ull)) * bar_ticks;

  do {
    _instrument = Instrument::random();
  } while(!is_valid_instrument(*_instrument));
  const uint8_t ambitus_half_range(Rand::next(6, 10));
  const uint8_t instrument_range(_instrument->max_tone() - _instrument->min_tone());

  {
    const uint8_t mid_tone(_instrument->min_tone() + ambitus_half_range + Rand::next_normal()*float(instrument_range - ambitus_half_range * 2));
    _min_tone = std::max<uint8_t>(mid_tone - ambitus_half_range, _instrument->min_tone());
    _max_tone = std::min<uint8_t>(mid_tone + ambitus_half_range, _instrument->max_tone());
    assert(_max_tone - _min_tone >= 12);
  }

  _min_time = NoteValue(Rand::next(sixteenth, quarter));
  _max_time = NoteValue(Rand::next(_min_time, whole));
  _repetition = Rand::next_float()*1.25f;
}
Notes Creator::compose() {
  uint32_t i(0);
  Notes notes;
  while(i < _music->size()) {
    bool new_figure_needed(true); // If it isn't changed it means no fitting figure was found
    for(uintptr_t figure_index(0); figure_index < _figures.size(); figure_index++) { // Iterate through figures already created
      const Figure& figure(_figures[figure_index]);
      if(i%figure.size == 0 // Its size is good for the place it would be in
        && i + figure.size <= _music->size() // The figure isn't too long
        && Rand::next_float() < _repetition // Add some randomness
        && harmony(_music->tones().data() + i, figure.tones.data(), figure.tones.size())) { // It's in harmony with the current music
        paste(figure.notes, notes, i); // Paste the figure on the music
        _figure_list.push_back(figure_index); // Register that we used this figure
        i += figure.size; // Go forth
        new_figure_needed = false; // Specify that the program doesn't need to create a new figure
        break;
      }
    }
    if(new_figure_needed) { // Needs to create a new figure of music
      Figure figure;
      figure.size = _figure_size;
      while(i%figure.size != 0 || i + figure.size > _music->size()) // Good size and not too long
        figure.size /= 2;
      figure.notes = get(i, figure.size); // Create the figure
      figure.tones = octave_tones(figure.notes);
      _figure_list.push_back(_figures.size()); // Register that we used this figure
      _figures.push_back(figure); // Add it to the collection of figures
      paste(figure.notes, notes, i); // Paste it on the music
      i += figure.size; // Go forth
    }
  }
  return notes;
}
bool Creator::is_valid_instrument(const Instrument&) const {
  return true;
}
void Creator::write_txt(std::ostream& s) const {
  s << "\t" << name() << " (" << _instrument->name() << ")" << std::endl
    << "\t\t[" << note_value_name(_min_time) << ":" << note_value_name(_max_time) << "]" << std::endl
    << "\t\t[" << note_name(_min_tone) << ":" << note_name(_max_tone) << "]" << std::endl
    << "\t\tRepetition factor: " << _repetition << std::endl;

  {
    s << "\t\tFigures:";
    for(uintptr_t figure_index : _figure_list) {
      s << " ";
      if(figure_index < 10) {
        s << figure_index;
      } else {
        s << char('a' + figure_index - 10);
      }
      const Figure& figure(_figures[figure_index]);
      const uint32_t bar_count(figure.size / bar_ticks);
      for(uint32_t i(1); i < bar_count; i++) {
        s << " -";
      }
    }
    s << std::endl;
  }
}
size_t Creator::time(size_t i, size_t size) const {
  size_t wtr(1 << Rand::next(_min_time, _max_time)); // 2^n ticks
  while(i%wtr != 0 || i + wtr > size) { // Good size and not too long
    wtr /= 2;
  }
  if(wtr < (1 << _max_time) && wtr > (1 << _min_time) &&
    (i / bar_ticks) == ((i + wtr + wtr / 2) / bar_ticks) &&
    Rand::next(0ull, 32ull / wtr)==0) {
    wtr += wtr / 2;
  }
  return wtr;
}
