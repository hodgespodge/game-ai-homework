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
    float windowSizeFactor;

    bool wheelOfFortune;
    int spinTime;
    int spinTimeMax;
    float maxSpinSpeed;

    int driveTime;
    int driveTimeMax;
    float maxDriveSpeed;

public:

    // Constructor
    Wander(sf::RenderWindow& window)
    {
        this->window = &window;

        // set the default user values ---------------------------------------------------
        numBoids = 1;
        numBreadCrumbs = 200;
        drawBreadcrumbs = true;
        fadeBreadcrumbs = true;
        drawID = false;
        // end of user values ------------------------------------------------------------

        windowSizeFactor = (window.getSize().x * window.getSize().y) / (float) IDEAL_WINDOW_SIZE;

        wheelOfFortune = false;

        spinTime = 0; 
        spinTimeMax = 5000;
        maxSpinSpeed = 3.0f;

        driveTime = 0;
        driveTimeMax = 1500;
        maxDriveSpeed = 0.3f;
        
 
    }

    void retarget()
    {
        // Randomly pick a new target position
        wheelOfFortune = true;
        
        // set spinTime to a random number between 0 and 150
        spinTime = rand() % 500;

    }

    void updateSprite(Boid& sprite, float elapsedTime)
    {

        sprite.updateBreadcrumbs();    

    

        if (wheelOfFortune){
            spinTime += elapsedTime;
            sprite.updateAngle(elapsedTime);

            if (spinTime > spinTimeMax){
                wheelOfFortune = false;
                sprite.angularVelocity = 0;
                
            }else{
                sprite.angularVelocity = (float)(spinTimeMax - spinTime)/1000;
                if (sprite.angularVelocity > maxSpinSpeed){
                    sprite.angularVelocity = maxSpinSpeed;
                }
            }
        }else{
            driveTime += elapsedTime;
            sprite.updatePosition(elapsedTime);
            sprite.teleportBetweenWalls(*window);


            if (driveTime > driveTimeMax){
                driveTime = rand() % 500;
                retarget();
            }else{
                sprite.linearVelocity = sf::Vector2f(1,1) * (float)(driveTimeMax - driveTime)/ 1000.0f;
                sprite.snapVelocityToAngle();
                if (magnitude(sprite.linearVelocity) > maxDriveSpeed){
                    sprite.linearVelocity = sprite.linearVelocity / magnitude(sprite.linearVelocity) * maxDriveSpeed;
                }
            }
        
        }
        
        
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