#ifndef ARRIVEANDALIGN_H
#define ARRIVEANDALIGN_H

#include <SFML/Graphics.hpp>
#include "SteeringBehavior.h"
#include "HelperFunctions.h"
#include "Boid.h"

class ArriveAndAlign : public SteeringBehavior
{
private:
    sf::RenderWindow* window;
    sf::Vector2f TargetPosition;
    bool isTargetSet;
    float maxSpeed;
    float maxAcceleration;
    
    float linearTargetRadius;
    float linearSlowRadius;

    float timeToLinearTarget;

    float maxRotation;
    float maxAngularAcceleration;

    float angularTargetRadius; // when to stop rotating
    float angularSlowRadius; // not a true radius, but a threshold for when to slow angular motion

    // float timeToAngularTarget;

    float windowSizeFactor;

public:

    // Constructor
    ArriveAndAlign(sf::RenderWindow& window  )
    {
        this->window = &window;

        // set the default user values ---------------------------------------------------
        numBoids = 1;
        numBreadCrumbs = 40;
        drawBreadcrumbs = true;
        fadeBreadcrumbs = true;
        drawID = false;
        // end of user values ------------------------------------------------------------

        windowSizeFactor = (window.getSize().x * window.getSize().y) / (float) IDEAL_WINDOW_SIZE;

        isTargetSet = false;
        TargetPosition = sf::Vector2f(0,0);

        maxSpeed = 0.5f * windowSizeFactor; 
        maxAcceleration = 0.005f * windowSizeFactor;
        
        linearTargetRadius = 25.0f * windowSizeFactor;
        linearSlowRadius = 300.0f * windowSizeFactor;

        maxRotation = 0.05f;
        maxAngularAcceleration = 0.0005f;

        angularTargetRadius = 10.0f ;
        angularSlowRadius = 90.0f ;

    }

    void updateSprite(Boid& sprite, float elapsedTime)
    {

        sprite.updateBreadcrumbs();

        if(!isTargetSet){
            sprite.linearVelocity = sf::Vector2f(0,0);
            return;
        }

        // Get the direction to the target
        sf::Vector2f direction = TargetPosition - sprite.getPosition();
        float distance = magnitude(direction);

        float targetSpeed;
        if (! (distance < linearTargetRadius)  ) // else linear component is within goal
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

            if (magnitude(linearAcceleration) > maxAcceleration)
            {
                sprite.linearAcceleration = unitVector(linearAcceleration) * maxAcceleration;
            }else{
                sprite.linearAcceleration = linearAcceleration;
            }

            sprite.updateLinearVelocity(elapsedTime);
            sprite.updatePosition(elapsedTime);
        } else {

            sprite.linearAcceleration = sf::Vector2f(0,0);
            sprite.linearVelocity = sf::Vector2f(0,0);
        }        

        // Do the align algorithm

        float targetRotation;
        float rotation =  atan2(direction.y, direction.x) * 180.0f / 3.14159f -  sprite.getRotation();

        // Map the rotation to the (-pi, pi) range
        if (rotation > 180.0f)
        {
            rotation -= 360.0f;
        }
        else if (rotation < -180.0f)
        {
            rotation += 360.0f;
        }

        float rotationSize = abs(rotation);
        if(! rotationSize < angularTargetRadius){

            if(rotationSize > angularSlowRadius){
                targetRotation = maxRotation;
            }

            else{
                targetRotation = maxRotation * (rotationSize / angularSlowRadius);
            }

            targetRotation *= rotation / rotationSize;

            sprite.angularAcceleration = targetRotation - sprite.angularVelocity;
            // sprite.angularAcceleration /= timeToAngularTarget;

            if(abs(sprite.angularAcceleration) > maxAngularAcceleration){
                sprite.angularAcceleration /= abs(sprite.angularAcceleration);
                sprite.angularAcceleration *= maxAngularAcceleration;
            }

            sprite.updateAngularVelocity(elapsedTime);
            sprite.updateAngle(elapsedTime);
    
        } else{
            sprite.angularAcceleration = 0.0f;
            sprite.angularVelocity = 0;
        }

    }

    void postUpdate() // not used
    {
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

    std::string getName()
    {
        return "ArriveAndAlign";
    }
};


#endif // ARRIVEANDALIGN_H