#ifndef VELOCITYMATCHING_H
#define VELOCITYMATCHING_H

#include <SFML/Graphics.hpp>
#include "SteeringBehavior.h"
// #include "KinematicSprite.h"

// Declare a class that implements the SteeringBehavior interface 
class VelocityMatching : public SteeringBehavior
{
private:
    sf::RenderWindow* window;
    sf::Vector2i previousMousePosition;

public:
    // Constructor
    VelocityMatching(sf::RenderWindow& window)
    {
        this->window = &window;
        previousMousePosition = sf::Mouse::getPosition(window);
    }

    void updateSprite(KinematicSprite& sprite, float elapsedTime)
    {
        // using the previouseMousePosition, current mouse position and elapsed time, calculate mouse velocity
        sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

        // get the mouseVelocity using the difference between the current and previous mouse position divided by the elapsed time
        sf::Vector2f mouseVelocity =  sf::Vector2f((mousePosition.x - previousMousePosition.x) / elapsedTime, (mousePosition.y - previousMousePosition.y) / elapsedTime);
        
        sprite.linearVelocity = mouseVelocity;
        sprite.updatePosition(elapsedTime);
        sprite.snapAngleToVelocity();
        
        
    } 
    void postUpdate()
    {
        previousMousePosition = sf::Mouse::getPosition(*window);
    }
    void checkEvent(sf::Event event) // not used
    {
    }

    // Destructor
    ~VelocityMatching()
    {
    }

};

#endif

