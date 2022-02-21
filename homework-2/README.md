# Homework 2

## The files:

This homework project's code is located in the following files:
- [Main.cpp](/Main.cpp)
  - The main SFML loop
  - Basic command line args for choosing steering behavior 
  - Variables for setting the target FPS, window size, and number of threads

- [SteeringBehavior.h](/SteeringBehavior.h)
  - Abstract class for steering behaviors
  - subclassed by:
    - [VelocityMatching.h](/VelocityMatching.h)
    - [Wander.h](/Wander.h)
    - [ArriveAndAlign.h](/ArriveAndAlign.h)
    - [Flocking.h](/Flocking.h)

- [VelocityMatching.h](/VelocityMatching.h)
  - Steering behavior for matching the velocity of mouse movement
  - Uses postUpdate function to calculate mouse velocity
  - 
- [Wander.h](/Wander.h)
  - Steering Behavior for wandering
  - My own design for wander behavior
  - Applies acceleration to the agent in the direction of a random target
  - Uses postUpdate to change the wander target (every n updates)
  - Agent bounces off walls
  
- [ArriveAndAlign.h](/ArriveAndAlign.h)
  - Arrive and Align steering behavior
  - "Arrive" linear movement and "Align" angular movement are calculated independently
  - The only steering behavior in which rotation does not necessarily match the heading
- 
- [Flocking.h](/Flocking.h)
  - Implementation of Craig Reynold's flocking "Boid" algorithm
  - Implements:
    - Cohesion
    - Alignment
    - Separation 
  - Stores a list of all the boids which is used to calculate if a boid is in range of another.
  - Repulses boids away from walls (but does not bounce)
  
- [KinematicSprite.h](/KinematicSprite.h)
  - Subclass of sf::Sprite
  - Stores velocity and acceleration of the agent (linear and angular)
  - Includes functions for updating the velocity using the acceleration and the delta time
  - Includes functions for updating the position using the velocity and the delta time
  - Subclassed by:
    - [Boid.h](/Boid.h)
  
- [Boid.h](/Boid.h)
  - Adds int ID member
  - Includes function for rendering breadcrumbs
  - Includes function for rendering the agent's ID as text for debugging purposes
  
- [HelperFunctions.h](/HelperFunctions.h)
  - Basic vector math functions (some unused)

- [BasicThreadPool.h](/BasicThreadPool.h)
  - Provides a basic thread pool for parallelizing boid updates
  - Unsure if it actually provides any speedup
  - Is known to cause problems with GLX and SFML on Linux with NVIDIA drivers.

## How to compile and run:

Makefile is included at [Makefile](/Makefile)

- To make (automatically cleans up old object files and executables):
  - `make`
- To Run:
  - velocity matching:
    - `./Main -v`
  - wander:
    - `./Main -w`
  - arrive and align:
    - `./Main -a`
  - Boids:
    - `./Main`   or   `./Main -b`

## Customization:

Params for each ot the Steering Behavior demos are located in their respective files. 

The main variables you may wish to modify are included in the constructor of each of the main steering behaviors

Example Boids:
- `numBoids = 50;`
- `numBreadCrumbs = 5;`
- `drawBreadcrumbs = true;`
- `fadeBreadcrumbs = true;`
- `drawID = false;`

Additionally you may wish to modify the following variables in the main loop

- `int windowWidth = 640;`
- `int windowHeight = 480;`
- `int targetFPS = 40;`
- `int numThreads = 1;`