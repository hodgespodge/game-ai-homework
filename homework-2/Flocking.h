#ifndef FLOCKING_H
#define FLOCKING_H

#include "Boid.h"
#include "SteeringBehavior.h"
#include <SFML/Graphics.hpp>

class Flocking : public SteeringBehavior
{

private:
    sf::RenderWindow* window;
    std::list<Boid>* allBoids;

    float visionRadius;
    float separationRadius;
    float maxSpeed;
    float maxAcceleration;
    float linearAcceleration;

    float alignmentWeight;
    float separationWeight;
    float cohesionWeight;

public:

    Flocking(sf::RenderWindow& window, std::list<Boid>& allBoids)
    {
        this->window = &window;
        this->allBoids = &allBoids;
        
        visionRadius = 200.0f;
        separationRadius = 100.0f;
        maxSpeed = 0.2f;
        maxAcceleration = 0.01f;
        linearAcceleration = 0.001f;

        // am currently getting nan with this config. Must test
        alignmentWeight = 0.5f;
        separationWeight = 0.5f;
        cohesionWeight = 0.1f;
    }

    std::list<Boid> getNeighbors(Boid& boid)
    {   // Returns a list of all boids within the vision radius not including the boid itself
        std::list<Boid> neighbors;
        for (std::list<Boid>::iterator it = allBoids->begin(); it != allBoids->end(); ++it)
        {
            if (it->getID() != boid.getID())
            {   
                sf::Vector2f diff = it->getPosition() - boid.getPosition();

                if (magnitude(diff) < visionRadius)
                {
                    neighbors.push_back(*it);
                }
            }
        }
        
        //print the list of neighbors
        for (std::list<Boid>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
        {
            printf("%d\n", it->getID());
        }

        printf("boid %d has %d neighbors\n", boid.getID(), neighbors.size());

        return neighbors;

    }

    // Doesn't seem to work great
    sf::Vector2f getAlignment(Boid& boid, std::list<Boid>& neighbors)
    {
        // sf::Vector2f alignment = boid.getPosition();
        sf::Vector2f alignment(0.0f, 0.0f);
        for (std::list<Boid>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
        {
            alignment += it->linearVelocity;
        }


        if(neighbors.size() == 0)
        {
            printf("No neighbors in alignment\n");
        }

        alignment /= (float) neighbors.size();
        return alignment;
    }

    sf::Vector2f getCohesion(Boid& boid, std::list<Boid>& neighbors)
    {
        sf::Vector2f cohesion(0.0f, 0.0f);
        for (std::list<Boid>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
        {
            cohesion += it->getPosition();
        }

        if(neighbors.size() == 0)
        {
            printf("No neighbors in cohesion");
        }

        cohesion /= (float) neighbors.size();
        return cohesion - boid.getPosition();
    }

    sf::Vector2f getSeparation(Boid& boid, std::list<Boid>& neighbors)
    {
        sf::Vector2f separation(0.0f, 0.0f);
        int boidsInRadius = 0;

        for (std::list<Boid>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
        {
            sf::Vector2f direction = boid.getPosition() - it->getPosition();

            if (magnitude(direction) < separationRadius)
            {
                separation += direction;
                boidsInRadius++;
            }
        }
        separation /= (float) boidsInRadius;



        // printf("sepration values %f %f\n", separation.x, separation.y);
        return separation;
    }

    void updateSprite(Boid& sprite, float elapsedTime)
    {
        
        sprite.updateBreadcrumbs();

        std::list<Boid> neighbors = getNeighbors(sprite);  // get neighbors

        // printf("neighbors: %d\n", neighbors.size());
        sf::Vector2f steeringForce;

        // is it reporting neighbors when there are none?
        if (neighbors.size() > 0)
        {

            sf::Vector2f alignment = getAlignment(sprite, neighbors);
            // printf("alignment: %f, %f\n", alignment.x, alignment.y);

            sf::Vector2f cohesion = getCohesion( sprite, neighbors) ;
            // printf("cohesion: %f, %f\n", cohesion.x, cohesion.y);

            sf::Vector2f separation = getSeparation( sprite, neighbors);
            // printf("separation: %f, %f\n", separation.x, separation.y);

            steeringForce = (alignment * alignmentWeight) + (cohesion * cohesionWeight) + (separation * separationWeight);


            sprite.linearVelocity += steeringForce * 0.01f;

            if (magnitude(sprite.linearVelocity) > maxSpeed)
            {
                sprite.linearVelocity = unitVector(sprite.linearVelocity) * maxSpeed;
            }
        }
        else
        {
            // set linear acceleration to opposite of current linear velocity
            sprite.linearVelocity = sprite.linearVelocity * 0.0f;
        }

        sprite.updatePosition(elapsedTime);
        sprite.snapAngleToVelocity();
    }

    void postUpdate()
    {   
    }

    void checkEvent(sf::Event event)
    {
    }




};  

#endif