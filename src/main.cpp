#include <cstdio>
#include <fstream>
#include <iostream>

#include "Chord.h"
#include "ConfigJson.h"
#include "Music.h"
#include "Scale.h"
#include "Steve.h"

using namespace steve;

int main(int argc, char** argv) {
  note_name_init();

  ConfigJson config;

  config.parse_file("../cfg/all.steve.json");
  config.compute_cache();

  while(true) {
    Music music(config);
    std::cout << "Created " << music.to_short_string() << std::endl;
    {
      std::ofstream fs(music.to_short_string() + ".mid", std::ofstream::binary);
      music.write_mid(fs);
    }
    {
      std::ofstream fs(music.to_short_string() + ".txt");
      music.write_txt(fs);
    }
  }
  return 0;
}
