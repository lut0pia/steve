#pragma once

#include <vector>
#include "Instrument.h"
#include "Scale.h"
#include "Steve.h"

namespace steve {
  class Creator;
  class Music {
  protected:
    Notes _notes;
    Tones _tones;
    std::vector<const Creator*> _creators;
    Scale _scale;
    uint32_t _tempo, _size;

  public:
    Music();
    ToneSet tones_at(size_t start, size_t size = 1) const;
    std::string to_short_string() const;
    void check() const;

    void write_mid(std::ostream&) const;
    void write_txt(std::ostream&) const;

    inline const Notes& notes() const { return _notes; }
    inline const Tones& tones() const { return _tones; }
    inline const Scale& scale() const { return _scale; }
    inline size_t size() const { return _size; }
    inline size_t bars() const { return _size/bar_ticks; }
    inline size_t parts() const { return _creators.size(); }
    inline const std::vector<const Creator*>& creators() const { return _creators; }
    inline size_t tempo() const { return _tempo; }
    inline size_t tickTime() const { return 60000/(_tempo*32); }
    inline size_t duration() const { return (tickTime()*_size)/1000; }
  };
}
