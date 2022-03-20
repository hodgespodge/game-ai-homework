# Homework 2

## The files:

This homework project's code is located in the following files:
- [Main.cpp](/Main.cpp)
  - The main SFML loop for the a* based maze navigator
  - ISSUE: If Main segfaults when loading the indoor map, re-run `python3 generateIndoorMap.py`. This happens for roughly 1 in 3 randomly generated maps. 

- [SearchFunctions.h](/SearchFunctions.h)
  - Graph search functions for shortest path

- [Heuristic.h](/Heuristic.h)
  - Abstract class for graph search heuristic functions
  - subclassed by:
    - [EuclideanHeuristic](/EuclideanHeuristic.h)
    - [DijkstraHeuristic](/DijkstraHeuristic.h)
    - [ManyPathsHeuristic](/ManyPathsHeuristic.h)
    - [ManhattanHeuristic](/ManhattanHeuristic.h)

- [GraphNode.h](/GraphNode.h)
  - Graph Node class including values used by the search functions

- [Graph.h](/Graph.h)
  - Functions for reading in csv graph data files and creating a graph of connected nodes
  - Can read in an adjacency list or an "indoorMap.csv" 
  
- [MapDrawer.h](/MapDrawer.h)
  - Class which pre-generates the indoor map as well as the node graph overlay for SFML

- [PathFollower.h](/PathFollower.h)
  - Subclass of SteeringBehavior (see [SteeringBehavior.h](/from-homework-2/SteeringBehavior.h))
  - Controls movement of agent in Indoor Map environment

- [generateDigraph.py](/scripts/generateDigraph.py)
  - Python script for generating "edges.csv" and "nodes.csv" files for testing graph search.
  - Capable of creating [random digraphs](/scripts/randomDigraph.py) or [geometric random digraphs](/scripts/randomGeometricDigraph.py)

- [generateIndoorMap.py](/scripts/generateIndoorMap.py)
  - Python script for randomly generating a csv representation of a walled interior space of many rooms
  - ISSUE: If Main segfaults when loading the indoor map, re-run `python3 generateIndoorMap.py`. This happens for roughly 1 in 3 randomly generated maps. 




## How to compile and run:

Makefile is included at [Makefile](/Makefile)

- To make (automatically cleans up old object files and executables):
  - `make`
- To Run:
  - `./Main`

## Customization:

Params for each ot the Steering Behavior demos are located in their respective files. 

You may wish to modify the following variables in the main loop

- `int windowWidth = 640;`
- `int windowHeight = 480;`
- `int targetFPS = 40;`