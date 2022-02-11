#ifndef ARRIVEANDALIGN_H
#define ARRIVEANDALIGN_H

#include <SFML/Graphics.hpp>
#include "SteeringBehavior.h"
#include "HelperFunctions.h"

class ArriveAndAlign : public SteeringBehavior
{
private:
    sf::RenderWindow* window;
    sf::Vector2f TargetPosition;
    bool isTargetSet;
    std::list<sf::Sprite> BreadCrumbs;

public:

    float maxSpeed;
    float maxAcceleration;
    
    float linearTargetRadius;
    float linearSlowRadius;

    float timeToLinearTarget;

    float maxRotation;
    float maxAngularAcceleration;

    float angularTargetRadius; // when to stop rotating
    float angularSlowRadius; // not a true radius, but a threshold for when to slow angular motion

    float timeToAngularTarget;

    // Constructor
    ArriveAndAlign(sf::RenderWindow& window  )
    {
        this->window = &window;
        isTargetSet = false;
        TargetPosition = sf::Vector2f(0,0);

        maxSpeed = 1.0f; // 1 is plenty fast
        maxAcceleration = 0.005f;
        
        linearTargetRadius = 30.0f;
        linearSlowRadius = 150.0f;

        timeToLinearTarget = 0.1f;

        maxRotation = 0.5f;
        maxAngularAcceleration = 0.005f;

        angularTargetRadius = 10.0f;
        angularSlowRadius = 50.0f;

        timeToAngularTarget = 0.1f;

    }

    void updateSprite(KinematicSprite& sprite, float elapsedTime)
    {

        if(!isTargetSet){
            return;
        }

        // Get the direction to the target
        sf::Vector2f direction = TargetPosition - sprite.getPosition();
        float distance = magnitude(direction);

        float targetSpeed;
        
        if (! distance < linearTargetRadius  ) // else linear component is within goal
        {
                // if we are outside the linearSlowRadius, then go max speed
            if (distance > linearSlowRadius)
            {
                targetSpeed = maxSpeed;
            }
            else
            {
                // if we are inside the linearSlowRadius, then go max speed * (distance / linearSlowRadius)
                targetSpeed = maxSpeed * (distance / linearSlowRadius);
            }

            // Calculate the target velocity
            sf::Vector2f targetVelocity = unitVector(direction) * targetSpeed;

            sf::Vector2f linearAcceleration = targetVelocity - sprite.linearVelocity;
            // linearAcceleration /= timeToLinearTarget;

            if (magnitude(linearAcceleration) > maxAcceleration)
            {
                sprite.linearAcceleration = unitVector(linearAcceleration) * maxAcceleration;
            }else{
                sprite.linearAcceleration = linearAcceleration;
            }

            sprite.updateLinearVelocity(elapsedTime);
            sprite.updatePosition(elapsedTime);
        } else {
            sprite.linearVelocity = sf::Vector2f(0,0);
        }        

        float targetRotation;
        
        // Get angle between the direction to the target and the direction of the sprite using cosine

        float rotationSize = angleBetween(sprite.linearVelocity, direction);

        if(! rotationSize < angularTargetRadius){
            if(rotationSize > angularSlowRadius){
                targetRotation = maxRotation;
            }else{
                targetRotation = maxRotation * (rotationSize / angularSlowRadius);
            }

            float angular = targetRotation - sprite.angularVelocity;
            angular /= timeToAngularTarget;

            float angularAcceleration = abs(angular);
            if (angularAcceleration > maxAngularAcceleration)
            {
                angular /= angularAcceleration;
                angular *= maxAngularAcceleration;
            } 

            sprite.angularAcceleration = angular;


            // These functions are broken for some reason :/
            // sprite.updateAngularVelocity(elapsedTime);
            // sprite.updateAngle(elapsedTime);
    
        } else{
            sprite.angularVelocity = 0;
        }

    }
    void postUpdate()
    {
        for(auto &s : BreadCrumbs){
            window->draw(s);
        }

    }

    void checkEvent(sf::Event event)
    {

        // Mouse click : set target position
        if (event.type == sf::Event::MouseButtonPressed)
        {
            TargetPosition = sf::Vector2f(sf::Mouse::getPosition(*window));
            
            isTargetSet = true;

        }
        
    }

    // Destructor
    ~ArriveAndAlign()
    {
    }

};


#endif // ARRIVEANDALIGN_H