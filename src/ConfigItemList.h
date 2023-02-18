#pragma once

#include <cstdint>
#include <vector>

#include "Chord.h"
#include "Instrument.h"
#include "Scale.h"

namespace steve {
  template <class T>
  class ConfigItemList {
  protected:
    std::vector<std::shared_ptr<T>> _items;
    std::vector<std::shared_ptr<T>> _allowed_items;

  public:
    inline auto get_all() const { return _items; }
    inline auto get_allowed() const { return _allowed_items; }
    void compute_cache();
    std::shared_ptr<T> get_item(const std::string& name);
  };

  template <class T>
  void ConfigItemList<T>::compute_cache() {
    bool use_whitelist = false;

    _allowed_items.clear();
    for(const auto& item : _items) {
      if(item->whitelisted) {
        use_whitelist = true;
      }
    }

    for(const auto& item : _items) {
      if(!item->blacklisted && (!use_whitelist || item->whitelisted)) {
        _allowed_items.push_back(item);
      }
    }
  }

  template <class T>
  std::shared_ptr<T> ConfigItemList<T>::get_item(const std::string& name) {
    for(auto& candidate : _items) {
      if(candidate->name == name) {
        return candidate;
      }
    }

    std::shared_ptr<T> desc(new T);
    _items.push_back(desc);
    desc->name = name;
    return desc;
  }
}
