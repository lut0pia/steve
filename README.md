# Steve

Steve is a C++ program that composes music and generates MIDI files from scratch.

## Assumptions
- Harmony is defined such as, at any given time (preferably at a measure granularity), the set of all tones played fits inside a chord
- Melody is defined by a scale (kept throughout an entire music and common to all instruments), all tones must be fit in the scale
- Rhythm is defined by the fact that notes can only have a power of two duration, and a note can only be placed on a position dividable by its duration

## Building

Steve uses [premake](https://github.com/premake/premake-core/wiki/Using-Premake) to create a project file that can then be used to build it (a binary for Windows is already present at the root of the repo).

You can also use the makefile at the root of the repo to build with g++.
