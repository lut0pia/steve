#pragma once

#include <cstdint>
#include <vector>

#include "Chord.h"
#include "ChordChange.h"
#include "ConfigItemList.h"
#include "ConfigRangedNumber.h"
#include "CreatorDescription.h"
#include "Instrument.h"
#include "Scale.h"
#include "TimeSignature.h"

namespace steve {
  class Config {
  protected:
    ConfigRangedNumber _tempo;
    ConfigRangedNumber _duration;
    ConfigItemList<TimeSignature> _signatures;
    ConfigItemList<CreatorDescription> _creators;
    ConfigItemList<ChordDescription> _chords;
    ConfigItemList<ScaleDescription> _scales;
    ConfigItemList<ChordChange> _chord_changes;
    ConfigItemList<Instrument> _instruments;

    std::shared_ptr<ChordChange> get_chord_change(const std::shared_ptr<ChordDescription>& source, const std::shared_ptr<ChordDescription>& target, uint8_t tone_shift);

  public:
    Config();
    void compute_cache();
    void list_scales(std::ostream&) const;

    std::vector<Chord> get_chords_inside(ToneSet tones) const;
    std::vector<Chord> get_progression(const Scale&, size_t) const;
    std::vector<std::shared_ptr<const CreatorDescription>> get_creators() const;

    inline const auto& get_tempo() const { return _tempo; }
    inline const auto& get_duration() const { return _duration; }
    inline const auto& get_signatures() const { return _signatures; }
    inline const auto& get_scales() const { return _scales; }
    inline const auto& get_instruments() const { return _instruments; }
  };
}
