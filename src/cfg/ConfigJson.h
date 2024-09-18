#pragma once

#include "Config.h"
#include "../ext/json.hpp"

namespace steve {
  using json = nlohmann::json;
  class ConfigJson : public Config {
  protected:
    std::vector<std::string> _directory_stack;

  public:
    void parse_file(const char* filepath);
    void parse_buffer(const char* buffer, size_t size);
    void parse_time_signatures(const json&);
    void parse_creators(const json&);
    void parse_chords(const json&);
    void parse_scales(const json&);
    void parse_chord_changes(const json&);
    void parse_chord_change(const json&, const std::string& src, const std::string& tar, const std::string& off);
    void parse_instruments(const json&);

    void parse_item(const json&, ItemDescription&);
  };
}
