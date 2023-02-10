#pragma once

#include "Config.h"
#include "ext/json.h"

namespace steve {
  class ConfigJson : public Config {
  public:
    void parse_file(const char* filepath);
    void parse_buffer(const char* buffer, size_t size);
    void parse_chords(const json_object_s*);
    void parse_scales(const json_object_s*);
    void parse_instruments(const json_object_s*);
  };
}
