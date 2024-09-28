#pragma once

#include <memory>

#include "../Steve.h"

namespace steve {
  class Creator {
  protected:
    class Music* _music;
    std::shared_ptr<const struct Instrument> _instrument;
    std::vector<Pattern> _patterns;
    std::vector<uintptr_t> _pattern_list;
    size_t _pattern_size = 1;
    NoteValue _min_time = NoteValue::sixtyfourth, _max_time = NoteValue::whole;
    uint8_t _min_tone = 0, _max_tone = 128;
    float _repetition = 1.f;
    uint8_t _channel;

  public:
    Creator(class Music*);
    virtual ~Creator() {}
    virtual void init();
    virtual void post_init();
    virtual Notes compose();
    virtual Notes get(size_t start, size_t size) const = 0;
    virtual const char* name() const = 0;
    virtual bool is_valid_instrument(const Instrument& instrument) const;
    virtual void add_note(Notes&, uint8_t tone, size_t start, size_t length, uint8_t velocity = 127) const;

    inline std::shared_ptr<const Instrument> get_instrument() const { return _instrument; }
    inline const std::vector<Pattern>& get_patterns() const { return _patterns; }
    inline const std::vector<uintptr_t>& get_pattern_list() const { return _pattern_list; }
    inline NoteValue get_min_time() const { return _min_time; }
    inline NoteValue get_max_time() const { return _max_time; }
    inline uint8_t get_min_tone() const { return _min_tone; }
    inline uint8_t get_max_tone() const { return _max_tone; }
    inline float get_repetition() const { return _repetition; }

    // Returns a valid amount of ticks until a next potential note
    uintptr_t generate_time(uintptr_t i, size_t size, bool chord_strict = false) const;
    // Returns a list of tick indices that partition time in a way
    // that notes can be played inbetween and still respect creator's time restrictions
    // and chord restrictions
    // @chord_strict means that a chord change REQUIRES a fracture even if there are common tones
    std::vector<uintptr_t> generate_times(uintptr_t start, size_t size, bool chord_strict = false) const;
  };
}
