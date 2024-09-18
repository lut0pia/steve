#pragma once

#include <memory>
#include <vector>

#include "Steve.h"
#include "cfg/Scale.h"
#include "cfg/TimeSignature.h"
#include "ctr/Creator.h"

namespace steve {
  class Config;
  class Creator;
  class Music {
  protected:
    const Config& _config;
    Notes _notes;
    Tones _tones;
    std::vector<std::unique_ptr<Creator>> _creators;
    std::vector<Chord> _chord_progression;
    std::vector<bool> _beats;
    Scale _scale;
    uint32_t _tempo, _size;
    std::shared_ptr<TimeSignature> _signature;

  public:
    Music(const Config&);
    void add_creator(Creator* creator);
    const Chord& chord_at(size_t i) const;
    ToneSet tones_at(size_t start, size_t size = 1) const;
    bool is_beat(uintptr_t i) const;
    std::vector<uintptr_t> beats_inside(uintptr_t min, uintptr_t max) const;
    std::string to_short_string() const;
    void check() const;

    inline const Config& get_config() const { return _config; }
    inline const Notes& get_notes() const { return _notes; }
    inline const Tones& tones() const { return _tones; }
    inline const Scale& get_scale() const { return _scale; }
    inline const std::vector<Chord> get_chord_progression() const { return _chord_progression; }
    inline std::vector<bool> get_beats() const { return _beats; }
    inline uint32_t get_size() const { return _size; }
    inline uint32_t bars() const { return _size / get_bar_ticks(); }
    inline uint32_t get_bar_ticks() const { return ticks_for(_signature->beat_value) * _signature->beats_per_bar; }
    inline NoteValue get_beat_value() const { return _signature->beat_value; }
    inline size_t parts() const { return _creators.size(); }
    inline const std::vector<std::unique_ptr<Creator>>& get_creators() const { return _creators; }
    inline uint32_t get_tempo() const { return _tempo; }
    inline const TimeSignature& get_signature() const { return *_signature; }
    inline size_t tickTime() const { return 60000 / (_tempo * 32); }
    inline size_t duration() const { return (tickTime() * _size) / 1000; }
  };
}
