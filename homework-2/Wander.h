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
    // int numBreadCrumbs;
    int updateCount;
    // int updatesPerBreadCrumb;
    // sf::Sprite breadCrumbSprite;
    // sf::Texture breadCrumbTexture;
    // std::list<sf::Sprite> BreadCrumbs;
    sf::Vector2f TargetPosition;

public:

    // Constructor
    Wander(sf::RenderWindow& window)
    {
        this->window = &window;

        // Initialize target position to the center of the window
        TargetPosition = sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2);

        // numBreadCrumbs = 20;
        updateCount = 0;
        // updatesPerBreadCrumb = 10;

        // breadCrumbTexture.loadFromFile("images/breadcrumb.png");

    }

    void updateSprite(Boid& sprite, float elapsedTime)
    {
        
        updateCount++;

        sprite.updateBreadcrumbs();

        float maxAcceleration = 0.001f;
        float maxSpeed = 0.7f;

        if (updateCount % 100 == 0)
        {
            // set targetPosition to a random position on the screen
            TargetPosition = sf::Vector2f(rand() % window->getSize().x, rand() % window->getSize().y);

        }

        // if sprite is out of the window, bounce the velocity back
        if (sprite.getPosition().x < 0 || sprite.getPosition().x > window->getSize().x)
        {
            sprite.linearVelocity= sf::Vector2f(sprite.linearVelocity.x * -1, sprite.linearVelocity.y);

            // also set the sprite's position back to the edge it hit
            if (sprite.getPosition().x < 0)
            {
                sprite.setPosition(0, sprite.getPosition().y);
            }
            else
            {
                sprite.setPosition(window->getSize().x, sprite.getPosition().y);
            }
        }

        if (sprite.getPosition().y < 0 || sprite.getPosition().y > window->getSize().y)
        {
            sprite.linearVelocity = sf::Vector2f(sprite.linearVelocity.x, sprite.linearVelocity.y * -1);

            // also set the sprite's position back to the edge it hit
            if (sprite.getPosition().y < 0)
            {
                sprite.setPosition(sprite.getPosition().x, 0);
            }
            else
            {
                sprite.setPosition(sprite.getPosition().x, window->getSize().y);
            }
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

        sprite.updateLinearVelocity(elapsedTime);
        sprite.updatePosition(elapsedTime);
        sprite.snapAngleToVelocity();
        
        
    } 
    void postUpdate()
    {

    }
    void checkEvent(sf::Event event) // not used
    {
    }

    // void drawExtra(sf::RenderWindow& window) // not used
    // {

    //     // int i = 0;
    //     // for (auto it = BreadCrumbs.begin(); it != BreadCrumbs.end(); ++it)
    //     // {
    //     //     // have breadcrumbs later in the list fade out
    //     //     it->setColor(sf::Color(0,0,0, 255 - i * (255/numBreadCrumbs)));

    //     //     window.draw(*it);
    //     //     i += 1;
    //     // }

    //     // for each sprite, drawBreadcrumbs
    //     for (auto it = BreadCrumbs.begin(); it != BreadCrumbs.end(); ++it)
    //     {
    //         window.draw(*it);
    //     }

    // }

    // Destructor
    ~Wander()
    {
    }

};

#endif