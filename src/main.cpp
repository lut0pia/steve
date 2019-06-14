#include <cstdio>
#include <iostream>
#include <fstream>
#include "Chord.h"
#include "Music.h"
#include "Scale.h"
#include "Steve.h"

using namespace steve;

int main() {
  note_name_init();

  // Chords
  Chord::add(Chord("Major", "", {0,4,7}));
  Chord::add(Chord("Minor", "m", {0,3,7}));
  Chord::add(Chord("Augmented", "aug", {0,4,8}));
  Chord::add(Chord("Diminished", "dim", {0,3,6}));

  // Scales
  Scale::add(Scale("Blues", {0,3,5,6,7}));
  //Scale::add(Scale("Byzantine",{0,1,4,5,7,8,11}));
  Scale::add(Scale("Harmonic minor", {0,2,3,5,7,8,11}));
  //Scale::add(Scale("Kumoi",{0,2,3,7,9}));
  Scale::add(Scale("Major", {0,2,4,5,7,9,11}));
  Scale::add(Scale("Minor", {0,2,3,5,7,8,10}));
  //Scale::add(Scale("Melodic minor",{0,2,3,5,7,9,11}));
  Scale::add(Scale("Pentatonic major", {0,2,4,7,9}));
  Scale::add(Scale("Whole tone", {0,2,4,6,8,10}));

  // Instruments
  // Tone numbering starts at C-1 (CCCC)
  Instrument::add(Instrument(0, "Acoustic Grand Piano", 21, 108));
  Instrument::add(Instrument(1, "Bright Acoustic Piano", 21, 108));
  Instrument::add(Instrument(2, "Electric Grand Piano", 21, 108));
  Instrument::add(Instrument(3, "Honky Tonk Piano", 21, 108));
  Instrument::add(Instrument(4, "Electric Piano 1", 21, 108));
  Instrument::add(Instrument(5, "Electric Piano 2", 21, 108));
  Instrument::add(Instrument(6, "Harpsichord", 21, 88));
  Instrument::add(Instrument(7, "Clavinet", 21, 108));
  Instrument::add(Instrument(8, "Celesta", 48, 112));
  Instrument::add(Instrument(9, "Glockenspiel", 72, 112));
  Instrument::add(Instrument(10, "Music Box", 21, 108));
  Instrument::add(Instrument(11, "Vibraphone", 48, 88));
  Instrument::add(Instrument(12, "Marimba", 36, 96));
  Instrument::add(Instrument(13, "Xylophone", 50, 108));
  Instrument::add(Instrument(14, "Tubular bells", 52, 80));
  Instrument::add(Instrument(15, "Dulcimer", 48, 84));
  Instrument::add(Instrument(16, "Drawbar Organ", 24, 84));
  Instrument::add(Instrument(17, "Percussive Organ", 24, 84));
  Instrument::add(Instrument(18, "Rock Organ", 24, 84));
  Instrument::add(Instrument(19, "Church Organ", 24, 84));
  Instrument::add(Instrument(20, "Reed Organ", 24, 84));
  //Instrument::add(Instrument(21, "Accordion", 44, 92));
  //Instrument::add(Instrument(22, "Harmonica", 60, 96));
  //Instrument::add(Instrument(23, "Tango Accordion", 44, 92));
  Instrument::add(Instrument(24, "Acoustic Guitar (nylon)", 40, 77));
  Instrument::add(Instrument(25, "Acoustic Guitar (steel)", 40, 77));
  Instrument::add(Instrument(26, "Electric Guitar (jazz)", 40, 77));
  Instrument::add(Instrument(27, "Electric Guitar (clean)", 40, 77));
  Instrument::add(Instrument(28, "Electric Guitar (muted)", 40, 77));
  //Instrument::add(Instrument(29, "Overdriven Guitar", 40, 77));
  //Instrument::add(Instrument(30, "Distorted Guitar", 40, 77));
  //Instrument::add(Instrument(31, "Guitar Harmonics", 40, 77));
  Instrument::add(Instrument(32, "Acoustic Bass", 24, 77));
  Instrument::add(Instrument(33, "Electric Fingered Bass", 24, 77));
  Instrument::add(Instrument(34, "Electric Picked Bass", 24, 77));
  Instrument::add(Instrument(35, "Fretless Bass", 24, 77));
  Instrument::add(Instrument(36, "Slap Bass 1", 24, 77));
  Instrument::add(Instrument(37, "Slap Bass 2", 24, 77));
  Instrument::add(Instrument(38, "Syn Bass 1", 24, 77));
  Instrument::add(Instrument(39, "Syn Bass 2", 24, 77));
  Instrument::add(Instrument(40, "Violin", 55, 103));
  Instrument::add(Instrument(41, "Viola", 48, 91));
  Instrument::add(Instrument(42, "Cello", 36, 76));
  Instrument::add(Instrument(43, "Contrabass", 28, 67));
  Instrument::add(Instrument(44, "Tremolo Strings", 36, 91));
  Instrument::add(Instrument(45, "Pizzicato Strings", 36, 91));
  Instrument::add(Instrument(46, "Orchestral Harp", 23, 100));
  Instrument::add(Instrument(47, "Timpani", 36, 62));
  Instrument::add(Instrument(48, "String Ensemble", 36, 91));
  Instrument::add(Instrument(49, "String Ensemble 2 (Slow)", 36, 91));
  Instrument::add(Instrument(50, "Syn Strings 1", 36, 91));
  Instrument::add(Instrument(51, "Syn Strings 2", 36, 91));
  Instrument::add(Instrument(52, "Choir Aahs", 40, 84));
  Instrument::add(Instrument(53, "Voice Oohs", 40, 84));
  Instrument::add(Instrument(54, "Syn Choir", 40, 84));
  Instrument::add(Instrument(55, "Orchestral Hit", 36, 91));
  Instrument::add(Instrument(56, "Trumpet", 55, 82));
  Instrument::add(Instrument(57, "Trombone", 40, 72));
  Instrument::add(Instrument(58, "Tuba", 28, 58));
  Instrument::add(Instrument(59, "Muted Trumpet", 55, 82));
  Instrument::add(Instrument(60, "French Horn", 34, 77));
  Instrument::add(Instrument(61, "Brass Section", 28, 67));
  Instrument::add(Instrument(62, "Synth Brass 1", 28, 67));
  Instrument::add(Instrument(63, "Synth Brass 2", 28, 67));
  Instrument::add(Instrument(64, "Soprano Sax", 56, 88));
  Instrument::add(Instrument(65, "Alto Sax", 50, 81));
  Instrument::add(Instrument(66, "Tenor Sax", 45, 76));
  Instrument::add(Instrument(67, "Baritone Sax", 38, 69));
  Instrument::add(Instrument(68, "Oboe", 59, 93));
  Instrument::add(Instrument(69, "English Horn", 52, 84));
  Instrument::add(Instrument(70, "Bassoon", 35, 76));
  Instrument::add(Instrument(71, "Clarinet", 36, 95));
  Instrument::add(Instrument(72, "Piccolo", 72, 108));
  Instrument::add(Instrument(73, "Flute", 60, 96));
  Instrument::add(Instrument(74, "Recorder", 60, 96));
  Instrument::add(Instrument(75, "Pan Flute", 36, 95));
  //Instrument::add(Instrument(76, "Bottle Blow", 36, 95));
  Instrument::add(Instrument(77, "Shakuhachi", 36, 95));
  //Instrument::add(Instrument(78, "Whistle", 60, 84));
  Instrument::add(Instrument(79, "Ocarina", 60, 96));

  while(true) {
    Music music;
    std::cout << "Created " << music.to_short_string() << std::endl;
    {
      std::ofstream fs(music.to_short_string()+".mid", std::ofstream::binary);
      music.write_mid(fs);
    }
    {
      std::ofstream fs(music.to_short_string()+".txt");
      music.write_txt(fs);
    }
  }
  return 0;
}
