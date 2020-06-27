#pragma once

#include "ChordBasedCreator.h"

namespace steve {
  class Arpeggio : public ChordBasedCreator {
  private:
    NoteValue _step;
  public:
    Arpeggio(Music*);
    virtual void init() override;
    virtual Notes get(size_t start, size_t size) const override;
    virtual const char* name() const override { return "Arpeggio"; }
    virtual void write_txt(std::ostream&) const override;
  };
}
