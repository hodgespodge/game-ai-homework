#ifndef WANDER_H
#define WANDER_H

#include "SteeringBehavior.h"
#include <SFML/Graphics.hpp>
#include "Boid.h"


// A wander behavior of my own design

// subclass of SteeringBehavior
class Wander : public SteeringBehavior
{
private:
    sf::RenderWindow* window;
    int updateCount;
    sf::Vector2f TargetPosition;

public:

    // Constructor
    Wander(sf::RenderWindow& window)
    {
        this->window = &window;

        // set the default user values
        numBoids = 1;
        numBreadCrumbs = 50;
        drawBreadcrumbs = true;
        fadeBreadcrumbs = true;
        drawID = false;
        // end of user values

        // Initialize target position to the center of the window
        TargetPosition = sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2);

        updateCount = 0;
 
    }

    void updateSprite(Boid& sprite, float elapsedTime)
    {
        
        updateCount++;

        sprite.updateBreadcrumbs();

        float maxAcceleration = 0.0005f;
        float maxSpeed = 0.4f;

        if (updateCount % 100 == 0)
        {
            // set targetPosition to a random position on the screen
            TargetPosition = sf::Vector2f(rand() % window->getSize().x, rand() % window->getSize().y);

        }        

        sf::Vector2f direction = TargetPosition - sprite.getPosition();
        direction = unitVector(direction);

        // if linearVelocity magnitude is less than maxSpeed, add acceleration
        if (magnitude(sprite.linearVelocity) < maxSpeed)
        {
            sprite.linearAcceleration = direction * maxAcceleration;
        } else {
            sprite.linearAcceleration = unitVector(sprite.linearVelocity) * -maxAcceleration;
        }

        sprite.updateLinearVelocity(elapsedTime, maxSpeed);
        sprite.bounceOffWalls(*window, 0.8f);
        sprite.updatePosition(elapsedTime);
        sprite.snapAngleToVelocity();
        
        
    } 
    void postUpdate()
    {

    }
    void checkEvent(sf::Event event) // not used
    {
    }

    // Destructor
    ~Wander()
    {
    }

};

#endif