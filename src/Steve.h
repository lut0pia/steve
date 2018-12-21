#pragma once

#include <map>
#include <set>
#include <vector>

namespace steve {
  struct Note {
    uint8_t channel : 4,
      stop : 1,
      tone : 7,
      velocity : 7;
  };
  enum NoteValue {
    sixtyfourth = 1,
    thirtysecond,
    sixteenth,
    eighth,
    quarter,
    half,
    whole
  };
  enum Interval {
    perfectunison = 0,
    minorsecond,
    majorsecond,
    minorthird,
    majorthird,
    perfectfourth,
    perfectfifth = 7,
    minorsixth,
    majorsixth,
    minorseventh,
    majorseventh,
    perfectoctave
  };
  typedef std::multimap<uint32_t, Note> Notes;
  struct Figure {
    Notes notes;
    size_t size;
  };
  typedef uint16_t ToneSet;
  typedef uint64_t NoteSet;
  typedef std::vector<ToneSet> Tones;
  static const size_t bar_ticks = 128; // ticks per bar
  inline uint32_t ticks_for(NoteValue v) { return uint32_t(pow(uint32_t(2), uint32_t(v))); }
  ToneSet shift(const ToneSet& scale, int shifting);
  const char* key_name(uint8_t);
  const char* note_value_name(uint8_t);
  void add_note(Notes&, uint8_t channel, uint8_t tone, size_t start, size_t length, uint8_t velocity = 127);

  Tones octave_tones(const Notes&);
  void paste(const Notes&, Notes&, size_t start = 0);
  Notes copy(const Notes&, size_t start = 0, size_t size = -1);
  bool harmony(size_t start, const std::vector<ToneSet>& base, const std::vector<ToneSet>& piece);
}
