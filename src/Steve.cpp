#include "Steve.h"

#include <iostream>
#include <cassert>

#include "Chord.h"

using namespace std;
using namespace steve;

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
const char* steve::note_value_name(NoteValue v) {
  switch(v) {
    case NoteValue::sixtyfourth: return "sixtyfourth";
    case NoteValue::thirtysecond: return "thirtysecond";
    case NoteValue::sixteenth: return "sixteenth";
    case NoteValue::eighth: return "eighth";
    case NoteValue::quarter: return "quarter";
    case NoteValue::half: return "half";
    case NoteValue::whole: return "whole";
  }
  return "N/A";
}

static char note_names[128 * 5];
void steve::note_name_init() {
  // Start at C-1
  // Worst case: C#-1\0 (5 bytes)
  for(uint8_t note(0); note < 128; note++) {
    sprintf(note_names + note * 5, "%s%d", key_name(note % 12), (note / 12) - 1);
  }
}
const char* steve::note_name(uint8_t note) {
  return note_names + note * 5;
}
uint8_t steve::get_note_with_name(const char* name) {
  for(uintptr_t note = 0; note < 128; note++) {
    if(!strcmp(note_names + note * 5, name)) {
      return uint8_t(note);
    }
  }
  return 0;
}
ToneSet steve::tone_set_shift(const ToneSet& tones, int shifting) {
  assert(shifting >= 0 && shifting < 12);
  return ((tones << shifting) | (tones >> (12 - shifting))) & 0xfff;
}
const char* steve::tone_set_binary(ToneSet tone_set) {
  static char str[13] = {};
  for(uint32_t tone(0); tone<12; tone++) {
    str[tone] = (tone_set & (1<<tone)) ? '1' : '0';
  }
  return str;
}
void steve::add_note(Notes& notes, uint8_t channel, uint8_t tone, size_t start, size_t length, uint8_t velocity) {
  Note note;
  note.channel = channel;
  note.tone = tone;
  note.velocity = velocity;
  note.duration = length;
  notes.insert(make_pair(start, note));
}

Tones steve::octave_tones(const Notes& notes) {
  Tones tones;
  for(const auto& note : notes) {
    while(tones.size() < note.first + note.second.duration) {
      tones.push_back(0);
    }
    for(uintptr_t i = 0; i < note.second.duration; i++) {
      if(note.second.channel != 9) { // Drums are atonal
        tones[note.first + i] |= 1 << (note.second.tone % 12);
      }
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
  auto it = src.lower_bound(start);
  while(it != src.end() && (*it).first <= start + size) {
    wtr.insert(make_pair((*it).first-start, (*it).second));
    it++;
  }
  return wtr;
}
bool steve::harmony(const ToneSet* base, const ToneSet* piece, size_t size) {
  for(uintptr_t i(0); i<size; i++) {
    if((base[i]|piece[i]) != base[i]) {
      return false;
    }
  }
  return true;
}
