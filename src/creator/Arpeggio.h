#pragma once

#include "ChordBasedCreator.h"

namespace steve {
  class Arpeggio : public ChordBasedCreator {
  private:
    NoteValue _step;
  public:
    Arpeggio(Music*);
    Notes get(size_t start, size_t size) const;
    const char* name() const override { return "Arpeggio"; }
    virtual void write_txt(std::ostream&) const override;
  };
}
