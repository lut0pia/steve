#include <cstdio>
#include <fstream>
#include <iostream>

#include "Music.h"
#include "Rand.h"
#include "Steve.h"
#include "cfg/ConfigJson.h"
#include "ext/cmdline.h"
#include "out/JsonOutput.h"
#include "out/MidiOutput.h"
#include "out/TextOutput.h"

using namespace steve;

int main(int argc, char** argv) {
  note_name_init();

  cmdline::parser args;
  args.set_program_name("steve");
  args.add("list-scales", 0, "list scales and their information");
  args.add("mid", 'm', "output .mid file");
  args.add("txt", 't', "output .txt file");
  args.add("json", 'j', "output .json file");
  args.add("random", 'r', "use random seed");
  args.add<std::string>("config", 'c', "path to a JSON configuration file", true);
  args.add<std::string>("out", 'o', "path to the files to write (w/o extension)", false);
  args.add<uint32_t>("num", 'n', "generation count", false, 1);
  args.add<uint32_t>("seed", 's', "seed", false, 0);
  args.parse_check(argc, argv);

  ConfigJson config;
  config.parse_file(args.get<std::string>("config").c_str());
  config.compute_cache();

  if(args.exist("list-scales")) {
    config.list_scales(std::cout);
    return 0;
  }

  uint32_t seed = args.get<uint32_t>("seed");
  if(args.exist("random")) {
    seed = time(nullptr);
  }
  Rand::generator.seed(seed);

  const uint32_t num = args.get<uint32_t>("num");
  const bool output_mid = args.exist("mid");
  const bool output_txt = args.exist("txt");
  const bool output_json = args.exist("json");
  for(uint32_t i = 0; i < num; i++) {
    std::string music_path = args.get<std::string>("out");
    Music music(config);
    if(music_path.empty()) {
      music_path = music.to_short_string();
    }
    std::cout << "Created " << music_path << std::endl;
    if(output_mid) {
      std::ofstream fs(music_path + ".mid", std::ofstream::binary);
      MidiOutput::write(music, fs);
    }
    if(output_txt) {
      std::ofstream fs(music_path + ".txt");
      TextOutput::write(music, fs);
    }
    if(output_json) {
      std::ofstream fs(music_path + ".json");
      JsonOutput::write(music, fs);
    }
  }

  return 0;
}
