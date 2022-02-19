#ifndef KINEMATICSPRITE_H
#define KINEMATICSPRITE_H

#include "HelperFunctions.h"
#include <SFML/Graphics.hpp>
#include <math.h>

class KinematicSprite : public sf::Sprite

{
public:
    sf::Vector2f linearVelocity;
    sf::Vector2f linearAcceleration;
    float angularVelocity;
    float angularAcceleration;

    // Constructor
    KinematicSprite()
    {
        linearVelocity = sf::Vector2f(0.0f, 0.0f);
        linearAcceleration = sf::Vector2f(0.0f, 0.0f);
        angularVelocity = 0.0f;
        angularAcceleration = 0.0f;
    }

    // Destructor
    ~KinematicSprite()
    {
    }

    // update the sprite's linearVelocity using its linearAcceleration
    void updateLinearVelocity(float elapsedTime)
    {
        linearVelocity += linearAcceleration * elapsedTime;
    }

    void updateLinearVelocity(float elapsedTime, float maxLinearVelocity)
    {
        linearVelocity += linearAcceleration * elapsedTime;

        if (magnitude(linearVelocity) > maxLinearVelocity)
        {
            
            linearVelocity = unitVector(linearVelocity) * maxLinearVelocity;
        }
    }
    // update the sprite's position using its linearVelocity
    void updatePosition(float elapsedTime)
    {
        move(linearVelocity * elapsedTime);
    }

    void updateAngularVelocity(float elapsedTime)
    {
        angularVelocity += angularAcceleration * elapsedTime;
    }

    void updateAngularVelocity(float elapsedTime, float maxAngularVelocity)
    {
        angularVelocity += angularAcceleration * elapsedTime;

        if (abs(angularVelocity) > maxAngularVelocity)
        {
            if (angularVelocity > 0)
            {
                angularVelocity = maxAngularVelocity;
            }
            else
            {
                angularVelocity = -maxAngularVelocity;
            }
        }
    }

    // update the sprite's angle using its angularVelocity
    void updateAngle(float elapsedTime)
    {
        rotate(angularVelocity * elapsedTime);
    }

    void snapAngleToVelocity()
    {
        if (magnitude(linearVelocity) > 0.0f)
        {
            setRotation(atan2(linearVelocity.y, linearVelocity.x) * 180.0f / 3.14159f);

        }
    }

    void bounceOffWalls(sf::RenderWindow& window){
        bounceOffWalls(window, 100.0f);
    }

    void bounceOffWalls(sf::RenderWindow& window, float bounceFactor)
    {

        int windowWidth = window.getSize().x;
        int windowHeight = window.getSize().y;
        sf::Vector2f position = getPosition();

        if (position.x < 0)
        {
            position.x = 1;

            if (linearVelocity.x < 0)
            {
                linearVelocity.x *= -bounceFactor;
            }
        }
        else if (position.x > windowWidth)
        {
            position.x = windowWidth - 1;

            if (linearVelocity.x > 0)
            {
                linearVelocity.x *= -bounceFactor;
            }
        }

        if (position.y < 0)
        {
            position.y = 1;
            if(linearVelocity.y < 0)
            {
                linearVelocity.y *= -bounceFactor;
            }
        
        }
        else if (position.y > windowHeight)
        {
            position.y = windowHeight - 1;
            if (linearVelocity.y > 0)
            {
                linearVelocity.y *= -bounceFactor;
            }
        }

    }

};

#endif