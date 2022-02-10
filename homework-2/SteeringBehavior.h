#ifndef STEERINGBEHAVIOR_H
#define STEERINGBEHAVIOR_H

#include "KinematicSprite.h"

// Declare an abstract class with a pure virtual function
class SteeringBehavior
{
public:
    virtual void updateSprite( KinematicSprite& sprite, float elapsedTime) = 0;
    virtual void postUpdate() = 0;
};


#endif