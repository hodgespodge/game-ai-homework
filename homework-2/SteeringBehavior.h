#ifndef STEERINGBEHAVIOR_H
#define STEERINGBEHAVIOR_H

// #include "KinematicSprite.h"
#include "Boid.h"

// Declare an abstract class with a pure virtual function
class SteeringBehavior
{
public:

    virtual void updateSprite( Boid& sprite, float elapsedTime) = 0;
    virtual void postUpdate() = 0;
    virtual void checkEvent(sf::Event event) = 0;
    // virtual void drawExtra(sf::RenderWindow& window) = 0;
};


#endif