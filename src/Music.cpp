#include "Music.h"

#include <iostream>

#include "Chord.h"
#include "creator.h"
#include "Scale.h"

using namespace steve;

void Music::add_part(const Creator& creator) {
  _tones = octave_tones(_notes);
  std::multimap<size_t, Notes> pieces;
  uint32_t i(0);
  const float repetition(creator.repetition());
  while(i<_size) {
    bool newPiece(true); // If it isn't changed it means no fitting piece was found
    for(auto it(pieces.rbegin()); it!=pieces.rend(); it++) // Iterate through pieces already created
      if(i%it->first==0 // Its size is good for the place it would be in
        && i+it->first<=_size // The piece isn't too long
        && Rand::next_float()<repetition // Add some randomness
        && harmony(i, _tones, octave_tones(it->second))) { // It's in harmony with the current music
        paste(it->second, _notes, i); // Paste the piece on the music
        i += it->first; // Go forth
        newPiece = false; // Specify that the program doesn't need to create a new piece
        break;
      }
    if(newPiece) { // Needs to create a new piece of music
      size_t piece_size(pow(2, Rand::next(0, 6))*bar_ticks); // 2^n bars
      while(i%piece_size!=0 || i+piece_size>_size) // Good size and not too long
        piece_size /= 2;
      Notes piece(creator.get(i, piece_size)); // Create the piece
      pieces.insert(make_pair(piece_size, piece)); // Add it to the collection of pieces
      paste(piece, _notes, i); // Paste it on the music
      i += piece_size; // Go forth
    }
  }
  //std::cout << "pieces: " << pieces.size() << std::endl;
}
uint8_t Music::add_channel(const Instrument* instrument) {
  uint8_t channel(_instruments.size());
  _instruments.push_back(instrument);
  return channel;
}
Music::Music() : _scale(Scale::random()), _tempo(240*Rand::gauss(5)) {
  do _size = Rand::next(20)+26;
  while(_size>512); // <=512 with 46 average bars
  _size *= bar_ticks;
  do {
    int mintime(Rand::next(eighth, half)), maxtime(Rand::next(mintime, whole));
    //cout << mintime << " " << maxtime << " " << minoct << " " << maxoct << endl;
    add_channel(Instrument::random());
    const Creator* creator;
    switch(Rand::next(0, 2)) {
      case 0:
        creator = new Melody(*this, mintime, maxtime, 3);
        break;
      case 1:
        mintime = std::min<int>(mintime + 1, whole);
        maxtime = std::min<int>(maxtime + 1, whole);
        creator = new Chords(*this, mintime, maxtime);
        break;
      case 2:
        mintime = std::min<int>(mintime + 1, whole);
        maxtime = std::min<int>(maxtime + 1, whole);
        creator = new Arpeggio(*this, mintime, maxtime);
        break;
    }
    _creators.push_back(creator);
    add_part(*creator);
  } while((parts()<2 || Rand::next(0, 1)) && _instruments.size()<9);
  if(Rand::next(0, 2)) {
    add_part(Drums(*this));
  }

#if _DEBUG
  _tones = octave_tones(_notes);
  for(ToneSet tones : _tones) {
    if(!Chord::harmony(tones)) {
      __debugbreak();
    }
  }
#endif
}
ToneSet Music::tones_at(size_t start, size_t size) const {
  ToneSet wtr(0);
  const uint32_t end(std::min(start+size, _tones.size()));
  for(uint32_t i(start); i<end; i++)
    wtr |= _tones[i];
  return wtr;
}
std::string Music::to_short_string() const {
  std::string short_string;
  short_string += scale().name() + "_";
  short_string += std::to_string(tempo());

  std::replace(short_string.begin(), short_string.end(), ' ', '_');

  return short_string;
}

static void write_bigendian(std::ostream& s, uint32_t v, uint32_t byteCount) {
  if(byteCount>=4) s << uint8_t(v>>24);
  if(byteCount>=3) s << uint8_t(v>>16);
  if(byteCount>=2) s << uint8_t(v>>8);
  if(byteCount>=1) s << uint8_t(v);
}
void Music::write_mid(std::ostream& s) const {
  // Header chunk
  s << "MThd"; // Chunk id
  write_bigendian(s, 6, 4); // Chunk size
  write_bigendian(s, 0, 2); // Format type (single track)
  write_bigendian(s, 1, 2); // Number of tracks
  write_bigendian(s, ticks_for(quarter), 2); // Time division (ticks per beat)
  // Track chunk
  s << "MTrk";
  const size_t sizeoff(s.tellp());
  write_bigendian(s, 0, 4); // Placeholder for track size
  s << uint8_t(0); // Track event delta time
  s << uint8_t(0xFF) << uint8_t(0x51) << uint8_t(3); // Tempo meta event
  write_bigendian(s, 60000000u/_tempo, 3); // Microseconds per quarter note
  for(uint32_t i(0); i<_instruments.size(); i++) {
    s << uint8_t(0) << uint8_t(0xC0|i) << _instruments[i]->midi_id(); // Program change
  }
  uint32_t last(0);
  for(auto&& note : _notes) {
    s << uint8_t(note.first-last); // Track event delta time
    s << uint8_t((note.second.stop ? 0x80 : 0x90)|note.second.channel);
    s << uint8_t(note.second.tone);
    s << uint8_t(note.second.velocity);
    last = note.first;
  }
  s << uint8_t(0) << uint8_t(0xFF) << uint8_t(0x2F) << uint8_t(0); // End of track
  // Write chunk size
  const size_t endoff(s.tellp());
  s.seekp(sizeoff);
  write_bigendian(s, endoff-sizeoff-4, 4);
}
void Music::write_txt(std::ostream& s) const {
  s << "Parts:" << std::endl;
  for(uintptr_t i(0); i<parts(); i++) {
    s << "\t- " << instruments()[i]->name() << " (" << creators()[i]->name() << ")" <<  std::endl;
  }
  s << "Scale: " << scale().name() << std::endl
    << "Tempo: " << tempo() << std::endl
    << "Duration: " << duration() << std::endl << std::endl;
}