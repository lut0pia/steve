#include "Music.h"

#include <iostream>

#include "Chord.h"
#include "creator.h"
#include "Scale.h"

using namespace Steve;

void Music::addPart(const Creator& creator) {
  std::multimap<size_t, Notes> pieces;
  std::vector<std::set<uint8_t> > tones(octaveTones(_notes));
  uint32_t i(0);
  float repetition(Rand::nextFloat());
  while(i<_size) {
    bool newPiece(true); // If it isn't changed it means no fitting piece was found
    for(auto it(pieces.rbegin()); it!=pieces.rend(); it++) // Iterate through pieces already created
      if(i%it->first==0 // Its size is good for the place it would be in
         && i+it->first<=_size // The piece isn't too long
         && Rand::nextFloat()<repetition // Add some randomness
         && harmony(i, tones, octaveTones(it->second))) { // It's in harmony with the current music
        paste(it->second, _notes, i); // Paste the piece on the music
        i += it->first; // Go forth
        newPiece = false; // Specify that the program doesn't need to create a new piece
        break;
      }
    if(newPiece) { // Needs to create a new piece of music
      size_t pieceSize(pow(2, Rand::next(0, 6))*barTicks); // 2^n bars
      while(i%pieceSize!=0 || i+pieceSize>_size) // Good size and not too long
        pieceSize /= 2;
      Notes piece(creator.get(i, pieceSize, tones)); // Create the piece
      pieces.insert(make_pair(pieceSize, piece)); // Add it to the collection of pieces
      paste(piece, _notes, i); // Paste it on the music
      i += pieceSize; // Go forth
    }
  }
  //std::cout << "pieces: " << pieces.size() << std::endl;
}
uint8_t Music::addChannel(const Instrument* instrument) {
  uint8_t channel(_instruments.size());
  if(channel==9) { // Avoid drum channel
    _instruments.push_back(NULL);
    channel++;
  } else if(channel>=16) {
    std::cerr << "Too many channels." << std::endl;
    exit(1);
  }
  _instruments.push_back(instrument);
  return channel;
}

Music::Music() : _scale(Scale::random()), _tempo(240*Rand::gauss(5)) {
  do _size = Rand::next(20)+26;
  while(_size>512); // <=512 with 46 average bars
  _size *= barTicks;
  do {
    const int mintime(Rand::next(eighth, whole)), maxtime(Rand::next(mintime, whole));
    //cout << mintime << " " << maxtime << " " << minoct << " " << maxoct << endl;
    const Instrument* instrument(Instrument::random());
    const uint8_t channel(addChannel(instrument));
    switch(Rand::next(0, 2)) {
      case 0:
        addPart(Melody(channel, _scale, *instrument, mintime, maxtime, 3));
        break;
      case 1:
        addPart(Chords(channel, _scale, *instrument, mintime, maxtime));
        break;
      case 2:
        addPart(Arpeggio(channel, _scale, *instrument, mintime, maxtime));
        break;
    }
  } while(parts()<2 || Rand::next(0, 1));
  if(Rand::next(0, 2))
    addPart(Drums());
}
static void writeBE(std::ostream& s, uint32_t v, uint32_t byteCount) {
  if(byteCount>=4) s << (uint8_t)(v>>24);
  if(byteCount>=3) s << (uint8_t)(v>>16);
  if(byteCount>=2) s << (uint8_t)(v>>8);
  if(byteCount>=1) s << (uint8_t)v;
}
void Music::write(std::ostream& s) const {
  // Header chunk
  s << "MThd"; // Chunk id
  writeBE(s, 6, 4); // Chunk size
  writeBE(s, 0, 2); // Format type (single track)
  writeBE(s, 1, 2); // Number of tracks
  writeBE(s, ticksFor(quarter), 2); // Time division (ticks per beat)
  // Track chunk
  s << "MTrk";
  const size_t sizeoff(s.tellp());
  writeBE(s, 0, 4); // Placeholder for track size
  s << (uint8_t)0; // Track event delta time
  s << (uint8_t)0xFF << (uint8_t)0x51 << (uint8_t)3; // Tempo meta event
  writeBE(s, 60000000u/_tempo, 3); // Microseconds per quarter note
  for(uint32_t i(0); i<_instruments.size(); i++)
    s << (uint8_t)0 << (uint8_t)(0xC0|i) << _instruments[i]->midiId(); // Program change
  uint32_t last(0);
  for(auto&& note : _notes) {
    s << (uint8_t)(note.first-last); // Track event delta time
    s << (uint8_t)((note.second.stop ? 0x80 : 0x90)|note.second.channel);
    s << (uint8_t)(note.second.tone+24) << (uint8_t)127;
    last = note.first;
  }
  s << (uint8_t)0 << (uint8_t)0xFF << (uint8_t)0x2F << (uint8_t)0; // End of track
  // Write chunk size
  const size_t endoff(s.tellp());
  s.seekp(sizeoff);
  writeBE(s, endoff-sizeoff-4, 4);
}


