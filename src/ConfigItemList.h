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
    float _total_weight = 0.f;

  public:
    inline auto get_all() const { return _items; }
    inline auto get_allowed() const { return _allowed_items; }
    void compute_cache();
    std::shared_ptr<T> get_item(const std::string& name);
    std::shared_ptr<T> get_random_item() const;
  };

  template <class T>
  void ConfigItemList<T>::compute_cache() {
    bool use_whitelist = false;

    _allowed_items.clear();
    _total_weight = 0.f;

    for(const auto& item : _items) {
      if(item->whitelisted) {
        use_whitelist = true;
      }
    }

    for(const auto& item : _items) {
      if(!item->blacklisted && (!use_whitelist || item->whitelisted)) {
        _allowed_items.push_back(item);
        _total_weight += item->weight;
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

  template <class T>
  std::shared_ptr<T> ConfigItemList<T>::get_random_item() const {
    float weight_index = Rand::next(0.f, _total_weight);
    for(const auto& candidate : _allowed_items) {
      if(weight_index < candidate->weight) {
        return candidate;
      } else {
        weight_index -= candidate->weight;
      }
    }
  }
}
