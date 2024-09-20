#pragma once

#include <memory>

#include "../Steve.h"

namespace steve {
  class Creator {
  protected:
    class Music* _music;
    std::shared_ptr<const struct Instrument> _instrument;
    std::vector<Phrase> _phrases;
    std::vector<uintptr_t> _phrase_list;
    size_t _phrase_size = 1;
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

    inline std::shared_ptr<const Instrument> get_instrument() const { return _instrument; }
    inline const std::vector<Phrase>& get_phrases() const { return _phrases; }
    inline const std::vector<uintptr_t>& get_phrase_list() const { return _phrase_list; }
    inline NoteValue get_min_time() const { return _min_time; }
    inline NoteValue get_max_time() const { return _max_time; }
    inline uint8_t get_min_tone() const { return _min_tone; }
    inline uint8_t get_max_tone() const { return _max_tone; }
    inline float get_repetition() const { return _repetition; }

    uintptr_t time(uintptr_t i, size_t size) const;
    std::vector<uintptr_t> generate_times(uintptr_t i, size_t size) const;
  };
}
