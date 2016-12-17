#pragma once

#include <vector>
#include "Creator.h"
#include "Instrument.h"

namespace Steve{
  class Music{
  protected:
    Notes _notes;
    std::vector<const Instrument*> _instruments;
    Scale _scale;
    uint32_t _tempo,_size;
    void addPart(const Creator&);
    uint8_t addChannel(const Instrument*);

  public:
    Music();
    void write(std::ostream&) const;

    inline const Notes& notes() const { return _notes; }
    inline const Scale& scale() const { return _scale; }
    inline size_t size() const { return _size; }
    inline size_t bars() const { return _size/barTicks; }
    inline size_t parts() const { return _instruments.size(); }
    inline const std::vector<const Instrument*>& instruments() const { return _instruments; }
    inline size_t tempo() const { return _tempo; }
    inline size_t tickTime() const { return 60000/(_tempo*32); }
    inline size_t duration() const { return (tickTime()*_size)/1000; }
  };
}
