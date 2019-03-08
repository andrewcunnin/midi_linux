# midi_linux
A project to explore Linux and midi

Currently this project just reads the midi data from a midi controller and doesn't do anything with it. There is commented code to generate sounds from the computer, but the sounds do not correspond to the key being pressed and I am working on a different implementation. The project is entirely specific to my hardware setup, right down to specifically looking at the port for dev/midi2, but ultimately I hope to make this more general as I learn more.

## Current To-do
- generate tones based on midi code frequency
- implement threads to make it polyphonic
- vary dynamics of notes based on midi data
