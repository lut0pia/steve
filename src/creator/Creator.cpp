#include "Creator.h"

#include "../Rand.h"

using namespace steve;

Creator::Creator(Music* music) {
  _music = music;
  _channel = music->creators().size();
  _instrument = Instrument::random();
  _mintime = Rand::next(sixteenth, half);
  _maxtime = Rand::next(_mintime, whole);
  _repetition = Rand::next_float();
}
Notes Creator::compose() {
  std::vector<ToneSet> tones = octave_tones(_music->notes());
  std::multimap<size_t, Notes> pieces;
  uint32_t i(0);
  Notes notes;
  while(i < _music->size()) {
    bool new_piece_needed(true); // If it isn't changed it means no fitting piece was found
    for(auto it(pieces.rbegin()); it != pieces.rend(); it++) // Iterate through pieces already created
      if(i%it->first == 0 // Its size is good for the place it would be in
        && i + it->first <= _music->size() // The piece isn't too long
        && Rand::next_float() < _repetition // Add some randomness
        && harmony(i, tones, octave_tones(it->second))) { // It's in harmony with the current music
        paste(it->second, notes, i); // Paste the piece on the music
        i += it->first; // Go forth
        new_piece_needed = false; // Specify that the program doesn't need to create a new piece
        break;
      }
    if(new_piece_needed) { // Needs to create a new piece of music
      size_t piece_size(pow(2, Rand::next(0, 6))*bar_ticks); // 2^n bars
      while(i%piece_size != 0 || i + piece_size > _music->size()) // Good size and not too long
        piece_size /= 2;
      Notes piece(get(i, piece_size)); // Create the piece
      pieces.insert(make_pair(piece_size, piece)); // Add it to the collection of pieces
      paste(piece, notes, i); // Paste it on the music
      i += piece_size; // Go forth
    }
  }
  return notes;
}
void Creator::write_txt(std::ostream& s) const {
  s << "\t" << name() << " (" << _instrument->name() << ")" << std::endl
    << "\t[" << note_value_name(_mintime) << ":" << note_value_name(_maxtime) << "]" << std::endl
    << "\tRepetition factor: " << _repetition << std::endl;
}
size_t Creator::time(size_t i, size_t size) const {
  size_t wtr(1 << Rand::next(_mintime, _maxtime)); // 2^n ticks
  while(i%wtr != 0 || i + wtr > size) // Good size and not too long
    wtr /= 2;
  return wtr;
}
