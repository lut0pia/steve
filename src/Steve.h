#pragma once

#include <map>
#include <set>
#include <vector>

namespace Steve{
  typedef struct{
    uint8_t channel : 4,
      stop : 1,
      tone : 7,
      velocity : 7;
  } Note;
  typedef enum{
    sixtyfourth = 1,
    thirtysecond,
    sixteenth,
    eighth,
    quarter,
    half,
    whole
  } NoteValue;
  typedef enum{
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
  } Interval;
  typedef std::multimap<uint32_t,Note> Notes;
  static const size_t barTicks = 128; // ticks per bar
  uint32_t ticksFor(NoteValue);
  std::set<uint8_t> shift(const std::set<uint8_t>& scale,int shifting);
  const char* keyName(uint8_t);
  std::set<uint8_t> octaveTones(const std::set<uint8_t>&);
  void addNote(Notes&,uint8_t channel,uint8_t tone,size_t start,size_t length,uint8_t velocity = 127);

  std::vector<std::set<uint8_t>> octaveTones(const Notes&);
  void paste(const Notes&,Notes&,size_t start = 0);
  Notes copy(const Notes&,size_t start = 0,size_t size = -1);
  bool harmony(size_t start,const std::vector<std::set<uint8_t>>& base,const std::vector<std::set<uint8_t>>& piece);

  template <class T> T merge(const T& a,const T& b){
    T wtr(a);
    wtr.insert(b.begin(),b.end());
    return wtr;
  }
}
