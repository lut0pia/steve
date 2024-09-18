#include "ConfigJson.h"

#include <sstream>

using namespace steve;

void parse_note_value(const json& object, const std::string& name, uint8_t& value) {
  if(object.contains(name)) {
    const json& note = object[name];
    if(note.is_string()) {
      value = get_note_with_name(note.get<std::string>().c_str());
    } else {
      value = note.get<int>();
    }
  }
}
template <class T>
void parse_value(const json& object, const std::string& name, T& value) {
  if(object.contains(name)) {
    value = object[name];
  }
}

void parse_ranged_number(const json& object, const std::string& name, ConfigRangedNumber& value) {
  if(object.contains(name)) {
    const json& number_object = object[name];
    parse_value(number_object, "min", value.min);
    parse_value(number_object, "max", value.max);
    parse_value(number_object, "step", value.step);
    parse_value(number_object, "sigma", value.sigma);
  }
}

void ConfigJson::parse_file(const char* relative_filepath) {
  std::string filepath;
  if(!_directory_stack.empty()) {
    filepath = _directory_stack.back() + relative_filepath;
  } else {
    filepath = relative_filepath;
  }

  {
    const size_t last_slash = filepath.find_last_of('/');
    std::string directory;
    if(last_slash >= 0) {
      directory = filepath.substr(0, last_slash + 1);
    }
    _directory_stack.push_back(directory);
  }

  FILE* file = fopen(filepath.c_str(), "rb");
  fseek(file, 0, SEEK_END);
  const size_t file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* file_data = (char*)malloc(file_size);
  fread(file_data, 1, file_size, file);

  parse_buffer(file_data, file_size);

  free(file_data);

  _directory_stack.pop_back();
}

void ConfigJson::parse_buffer(const char* buffer, size_t size) {
  json root = json::parse(buffer, buffer + size, nullptr, true, true);

  // The "parents" property needs to be parsed first
  if(root.contains("parents")) {
    for(const auto& parent_path : root["parents"]) {
      parse_file(parent_path.get<std::string>().c_str());
    }
  }
  parse_ranged_number(root, "tempo", _tempo);
  parse_ranged_number(root, "duration", _duration);
  if(root.contains("time_signatures")) {
    parse_time_signatures(root["time_signatures"]);
  }
  if(root.contains("creators")) {
    parse_creators(root["creators"]);
  }
  if(root.contains("chords")) {
    parse_chords(root["chords"]);
  }
  if(root.contains("scales")) {
    parse_scales(root["scales"]);
  }
  if(root.contains("chord_changes")) {
    parse_chord_changes(root["chord_changes"]);
  }
  if(root.contains("instruments")) {
    parse_instruments(root["instruments"]);
  }
}

void ConfigJson::parse_time_signatures(const json& time_signatures_object) {
  for(const auto& item : time_signatures_object.items()) {
    std::shared_ptr<TimeSignature> desc = _signatures.get_item(item.key());
    auto slash_pos = desc->name.find('/');
    if(slash_pos != std::string::npos) {
      desc->beats_per_bar = atoi(desc->name.substr(0, slash_pos).c_str());
      desc->beat_value = NoteValue(uint32_t(NoteValue::whole) - log2(atoi(desc->name.substr(slash_pos + 1).c_str())));
    }
    parse_item(item.value(), *desc);
  }
}

void ConfigJson::parse_creators(const json& creators_object) {
  for(const auto& item : creators_object.items()) {
    std::shared_ptr<CreatorDescription> desc = _creators.get_item(item.key());
    const json& creator_object = item.value();
    parse_item(creator_object, *desc);
    parse_value(creator_object, "min_count", desc->min_count);
    parse_value(creator_object, "max_count", desc->max_count);
  }
}

void ConfigJson::parse_chords(const json& chords_object) {
  for(const auto& item : chords_object.items()) {
    std::shared_ptr<ChordDescription> desc = _chords.get_item(item.key());
    const json& chord_object = item.value();
    parse_item(chord_object, *desc);
    parse_value(chord_object, "uppercase", desc->uppercase);
    parse_value(chord_object, "suffix", desc->suffix);
    if(chord_object.contains("tones")) {
      for(const json& tone : chord_object["tones"]) {
        desc->tones |= 1 << tone.get<int>();
      }
    }
  }
}

void ConfigJson::parse_scales(const json& scales_object) {
  for(const auto& item : scales_object.items()) {
    std::shared_ptr<ScaleDescription> desc = _scales.get_item(item.key());
    const json& scale_object = item.value();
    parse_item(scale_object, *desc);
    if(scale_object.contains("tones")) {
      for(const json& tone : scale_object["tones"]) {
        desc->tones |= 1 << tone.get<int>();
      }
    }
  }
}

void ConfigJson::parse_chord_changes(const json& chord_changes_object) {
  for(const auto& source_item : chord_changes_object.items()) {
    for(const auto& target_item : source_item.value().items()) {
      for(const auto& offset_item : target_item.value().items()) {
        parse_chord_change(offset_item.value(), source_item.key(), target_item.key(), offset_item.key());
      }
    }
  }
}

void ConfigJson::parse_chord_change(const json& chord_change_object, const std::string& source_string, const std::string& target_string, const std::string& offset_string) {
  std::vector<std::shared_ptr<ChordDescription>> source_chords;
  std::vector<std::shared_ptr<ChordDescription>> target_chords;
  std::vector<uint32_t> offsets;

  if(source_string == "*") {
    source_chords = _chords.get_all();
  } else {
    std::stringstream ss(source_string);
    for(std::string chord_name; std::getline(ss, chord_name, '|');) {
      source_chords.push_back(_chords.get_item(chord_name));
    }
  }

  if(target_string == "*") {
    target_chords = _chords.get_all();
  } else {
    std::stringstream ss(target_string);
    for(std::string chord_name; std::getline(ss, chord_name, '|');) {
      target_chords.push_back(_chords.get_item(chord_name));
    }
  }

  if(offset_string == "*") {
    for(uint32_t offset = 0; offset < 12; offset++) {
      offsets.push_back(offset);
    }
  } else {
    offsets.push_back(atoi(offset_string.c_str()));
  }

  for(const auto& source_chord : source_chords) {
    for(const auto& target_chord : target_chords) {
      for(uint32_t offset : offsets) {
        std::shared_ptr<ChordChange> chord_change = get_chord_change(source_chord, target_chord, offset);
        parse_item(chord_change_object, *chord_change);
      }
    }
  }
}

void ConfigJson::parse_instruments(const json& instruments_object) {
  for(const auto& item : instruments_object.items()) {
    std::shared_ptr<Instrument> desc = _instruments.get_item(item.key());
    const json& instrument_object = item.value();
    parse_item(instrument_object, *desc);
    parse_value(instrument_object, "index", desc->midi_id);
    parse_note_value(instrument_object, "min_tone", desc->min_tone);
    parse_note_value(instrument_object, "max_tone", desc->max_tone);
    parse_value(instrument_object, "voices", desc->voices);
  }
}

void ConfigJson::parse_item(const json& item_object, ItemDescription& item) {
  parse_value(item_object, "whitelist", item.whitelisted);
  parse_value(item_object, "blacklist", item.blacklisted);
  parse_value(item_object, "weight", item.weight);
}
