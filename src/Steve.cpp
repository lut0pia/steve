#include "Steve.h"

#include <algorithm>
#include <iostream>

#include "Chord.h"

using namespace std;
using namespace steve;

ToneSet steve::shift(const ToneSet& tones, int shifting) {
  ToneSet wtr(0);
  for(int i(0); i<12; i++) {
    if(tones & 1<<i) {
      wtr |= 1<<((i + shifting + 12)%12);
    }
  }
  return wtr;
}
const char* steve::key_name(uint8_t key) {
  switch(key) {
    case 0: return "C";
    case 1: return "C#";
    case 2: return "D";
    case 3: return "D#";
    case 4: return "E";
    case 5: return "F";
    case 6: return "F#";
    case 7: return "G";
    case 8: return "G#";
    case 9: return "A";
    case 10: return "A#";
    case 11: return "B";
  }
  return "N/A";
}
const char* steve::note_value_name(uint8_t value) {
  switch(value) {
    case sixtyfourth: return "sixtyfourth";
    case thirtysecond: return "thirtysecond";
    case sixteenth: return "sixteenth";
    case eighth: return "eighth";
    case quarter: return "quarter";
    case half: return "half";
    case whole: return "whole";
  }
  return "N/A";
}
void steve::add_note(Notes& notes, uint8_t channel, uint8_t tone, size_t start, size_t length, uint8_t velocity) {
  if(length>bar_ticks) {
    cerr << "Note too long." << endl;
    exit(1);
  }
  Note note;
  note.channel = channel;
  note.tone = tone;
  note.velocity = velocity;

  note.stop = 0;
  notes.insert(make_pair(start, note));
  note.stop = 1;
  notes.insert(make_pair(start+length, note));
}

Tones steve::octave_tones(const Notes& notes) {
  Tones tones;
  std::vector<std::set<uint8_t>> channel_tones(16);
  uint32_t i(0);
  for(const auto& note : notes) {
    if(note.second.channel != 9 && !note.second.stop) { // Not drums and play
      channel_tones[note.second.channel].insert(note.second.tone);
    }

    while(i < note.first) { // We've passed a tick
      ToneSet all_played(0);
      for(const std::set<uint8_t>& channel_played : channel_tones) {
        for(uint8_t tone : channel_played) {
          all_played |= 1 << tone % 12;
        }
      }
      tones.push_back(all_played);
      i++;
    }

    if(note.second.channel != 9 && note.second.stop) { // Not drums and stop
      channel_tones[note.second.channel].erase(note.second.tone);
    }
  }
  return tones;
}
void steve::paste(const Notes& src, Notes& tar, size_t start) {
  for(auto&& note : src)
    tar.insert(make_pair(note.first+start, note.second));
}
Notes steve::copy(const Notes& src, size_t start, size_t size) {
  Notes wtr;
  auto it(src.lower_bound(start));
  while(it != src.end() && (*it).first<=start+size) {
    if((*it).first==start) { // Only take the starts from the beginning
      if(!(*it).second.stop)  wtr.insert(make_pair((*it).first-start, (*it).second));
    } else if((*it).first==start+size) { // Only take the stops from the ending
      if((*it).second.stop)   wtr.insert(make_pair((*it).first-start, (*it).second));
    } else wtr.insert(make_pair((*it).first-start, (*it).second));
    it++;
  }
  return wtr;
}
bool steve::harmony(size_t start, const std::vector<ToneSet>& base, const std::vector<ToneSet>& piece) {
  for(uint32_t i(start); i<min(base.size(), start+piece.size()); i++) {
    if(!Chord::harmony(base[i]|piece[i-start]))
      return false;
  }
  return true;
}
