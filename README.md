# Steve

Steve is a very talented (!) MIDI music composer.

## Origins

Back in high school, I was fascinated by procedural generation, and so as I knew a little programming, I tried to create generating programs. That's when I started working on this one, even though this is probably the third or fourth iteration.

I was not (and still am not) in any way a music composer, I've never even had a music composing course in my life and so the whole thing is based on assumptions I've made about what composing music mathematically could be like. Meaning if you're an actual composer/programmer and think the way Steve composes is outrageous, first of all you're probably right, second of all I'd be glad to hear from you about how to make it better!

## Assumptions
- Harmony is defined such as, at any given time, the set of all tones played fits inside a chord
- Melody is defined by a scale (kept throughout an entire music and common to all instruments), all tones must be fit in the scale
- Rhythm is defined by the fact that notes can only have a power of two duration, and a note can only be placed on a position dividable by its duration

## Building

Steve uses [premake](https://github.com/premake/premake-core/wiki/Using-Premake) to create a project file that can then be used to build it (a binary for Windows is already present at the root of the repo).
