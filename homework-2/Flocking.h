#ifndef FLOCKING_H
#define FLOCKING_H

#include "Boid.h"
#include "SteeringBehavior.h"
#include <SFML/Graphics.hpp>

class Flocking : public SteeringBehavior
{

private:
    sf::RenderWindow* window;
    std::list<Boid*>* allBoids;

    float visionRadius;
    float separationRadius;
    float maxSpeed;
    float maxChange;

    float alignmentWeight;
    float separationWeight;
    float cohesionWeight;
    float windowWeight;

    float windowRepulsionRadius;
    float windowSizeFactor;

public:

    Flocking(sf::RenderWindow& window, std::list<Boid*>& allBoids)
    {
        this->window = &window;
        this->allBoids = &allBoids;

        // set the default user values
        numBoids = 100;
        numBreadCrumbs = 5;
        drawBreadcrumbs = true;
        fadeBreadcrumbs = true;
        drawID = false;
        // end of user values

        // Speed and radius variables are roughly scaled to window size so that flocking behavior is (mostly) consistent across resolutions
        windowSizeFactor = (window.getSize().x * window.getSize().y) / (float) IDEAL_WINDOW_SIZE;

        visionRadius = 90.0f * windowSizeFactor;
        separationRadius = 40.0f * windowSizeFactor;
        maxSpeed = 0.3f * windowSizeFactor;
        maxChange = 0.01f * windowSizeFactor; // Basically acceleration

        alignmentWeight = 1.2f;
        separationWeight = 0.8f;
        cohesionWeight = 0.75f;
        
        windowRepulsionRadius = 75.0f * windowSizeFactor;
        windowWeight = 0.015f * windowSizeFactor;

    }

    // Returns a list of all boids within the vision radius not including the boid itself
    std::list<Boid> getNeighbors(Boid& boid)
    {  
        std::list<Boid> neighbors;
        for (auto s : *allBoids)
        {
            if (s->getID() != boid.getID())
            {   
                sf::Vector2f diff = s->getPosition() - boid.getPosition();

                if (magnitude(diff) < visionRadius)
                {
                    neighbors.push_back(*s);
                }
            }
        }

        return neighbors;

    }

    // align the boid to the average heading of its neighbors
    sf::Vector2f getAlignment(Boid& boid, std::list<Boid>& neighbors)
    {
        // sf::Vector2f alignment = boid.getPosition();
        sf::Vector2f alignment(0.0f, 0.0f);
        for (std::list<Boid>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
        {
            alignment += it->linearVelocity;
        }

        alignment /= (float) neighbors.size();
        return alignment;
    }

    // move the boid towards the center of the neighbors
    sf::Vector2f getCohesion(Boid& boid, std::list<Boid>& neighbors)
    {
        sf::Vector2f cohesion(0.0f, 0.0f);
        for (std::list<Boid>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
        {
            cohesion += it->getPosition();
        }

        cohesion /= (float) neighbors.size();
        return cohesion - boid.getPosition();
    }

    // move the boid away from the neighbors (within a radius)
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
        if (boidsInRadius > 0)
        {
            separation /= (float) boidsInRadius;
        }

        return separation;
    }

    void updateSprite(Boid& sprite, float elapsedTime)
    {
        sprite.updateBreadcrumbs();

        std::list<Boid> neighbors = getNeighbors(sprite);  // get neighbors
        sf::Vector2f change = sf::Vector2f(0.0f, 0.0f);

        // is it reporting neighbors when there are none?
        if (neighbors.size() > 0)
        {

            sf::Vector2f alignment = getAlignment(sprite, neighbors);
            // printf("alignment: %f, %f\n", alignment.x, alignment.y);

            sf::Vector2f cohesion = getCohesion( sprite, neighbors) ;
            // printf("cohesion: %f, %f\n", cohesion.x, cohesion.y);

            sf::Vector2f separation = getSeparation( sprite, neighbors);
            // sf::Vector2f separation = sf::Vector2f(0.0f, 0.0f);

            change = (alignment * alignmentWeight)
                    + (cohesion * cohesionWeight)
                    + (separation * separationWeight);

            if (magnitude(change) > maxChange)
            {
                change = unitVector(change) * maxChange;
            }
        }

        sprite.linearVelocity += change;

        // Gently keep the boid within the window
        sprite.repulseOffWalls(*window, windowWeight, windowRepulsionRadius);

        if (magnitude(sprite.linearVelocity) > maxSpeed)
        {
            sprite.linearVelocity = unitVector(sprite.linearVelocity) * maxSpeed;
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

    std::string getName()
    {
        return "Flocking";
    }


};  

#endif