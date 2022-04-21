# Homework 2

- KNOWN ISSUE: If Main segfaults when loading the indoor map, re-run `python3 generateIndoorMap.py`. 

## How to compile and run:

Makefile is included at [Makefile](/Makefile)

- To make (automatically cleans up old object files and executables):
  - `make`
- To Run:
  - `./Main` Run the interactive maze navigator (and enemy)

## Controls:

- To set the mouse's cheese location (this will clear any cheese waypoints):
  - left click somewhere on the map

- To queue up multiple cheese waypoints:
  - Hold shift while left clicking on the map

## Customization:

You can procedurally generate a new map by running `python3 generateIndoorMap.py`. There's a ~30% chance the new map will cause a segfault when running `./Main` so you may need to try more than once.

You may wish to modify the following variables in the main loop

- `int windowWidth = 1600;`
- `int windowHeight = 900;`
- `int targetFPS = 40;`