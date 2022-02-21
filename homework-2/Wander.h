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
    float windowSizeFactor;

public:

    int updatesPerRetargetting = 100;

    // Constructor
    Wander(sf::RenderWindow& window)
    {
        this->window = &window;

        // set the default user values
        numBoids = 1;
        numBreadCrumbs = 100;
        drawBreadcrumbs = true;
        fadeBreadcrumbs = true;
        drawID = false;
        // end of user values

        updatesPerRetargetting = 100;

        windowSizeFactor = (window.getSize().x * window.getSize().y) / (float) IDEAL_WINDOW_SIZE;


        // Initialize target position to the center of the window
        TargetPosition = sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2);

        updateCount = 0;
        
 
    }

    void updateSprite(Boid& sprite, float elapsedTime)
    {
        
        updateCount++;

        sprite.updateBreadcrumbs();

        float maxAcceleration = 0.0005f * windowSizeFactor;
        float maxSpeed = 0.4f * windowSizeFactor;

        if (updateCount > updatesPerRetargetting)
        {
            // set targetPosition to a random position on the screen
            TargetPosition = sf::Vector2f(rand() % window->getSize().x, rand() % window->getSize().y);
            updateCount = 0;

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

    std::string getName()
    {
        return "Wander";
    }

};

#endif