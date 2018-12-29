#include "Creator.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include "../Rand.h"

using namespace steve;

Creator::Creator(Music* music) {
  _music = music;
  _channel = music->creators().size();
  _instrument = Instrument::random();
  const uint8_t mid_tone(_instrument->min_tone() + Rand::gauss(5)*float(_instrument->max_tone() - _instrument->min_tone()));
  _min_tone = std::max<uint8_t>(mid_tone - 10, _instrument->min_tone());
  _max_tone = std::min<uint8_t>(mid_tone + 10, _instrument->max_tone());
  _min_time = NoteValue(Rand::next(sixteenth, half));
  _max_time = NoteValue(Rand::next(_min_time, whole));
  _repetition = Rand::next_float()*2.f;

  assert(_max_tone - _min_tone > 12);
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
        && harmony(_music->tones().data()+i, figure.tones.data(), figure.size)) { // It's in harmony with the current music
        paste(figure.notes, notes, i); // Paste the figure on the music
        _figure_list.push_back(figure_index); // Register that we used this figure
        i += figure.size; // Go forth
        new_figure_needed = false; // Specify that the program doesn't need to create a new figure
        break;
      }
    }
    if(new_figure_needed) { // Needs to create a new figure of music
      Figure figure;
      figure.size = (1 << Rand::next(0, 6))*bar_ticks; // 2^n bars
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
void Creator::write_txt(std::ostream& s) const {
  s << "\t" << name() << " (" << _instrument->name() << ")" << std::endl
    << "\t[" << note_value_name(_min_time) << ":" << note_value_name(_max_time) << "]" << std::endl
    << "\t[" << note_name(_min_tone) << ":" << note_name(_max_tone) << "]" << std::endl
    << "\tRepetition factor: " << _repetition << std::endl;

  {
    s << "\tFigures:";
    for(uintptr_t figure_index : _figure_list) {
      s << " " << figure_index;
    }
    s << std::endl;
  }
}
size_t Creator::time(size_t i, size_t size) const {
  size_t wtr(1 << Rand::next(_min_time, _max_time)); // 2^n ticks
  while(i%wtr != 0 || i + wtr > size) // Good size and not too long
    wtr /= 2;
  return wtr;
}
