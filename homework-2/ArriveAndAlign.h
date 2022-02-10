#ifndef ARRIVEANDALIGN_H
#define ARRIVEANDALIGN_H

#include <SFML/Graphics.hpp>
#include "SteeringBehavior.h"

class ArriveAndAlign : public SteeringBehavior
{
private:

public:
    // Constructor
    ArriveAndAlign(sf::RenderWindow& window)
    {
        this->window = &window;
        previousMousePosition = sf::Mouse::getPosition(window);
    }

    void updateSprite(sf::Sprite& sprite, float elapsedTime)
    {

    }
    void postUpdate()
    {

    }

    // Destructor
    ~ArriveAndAlign()
    {
    }

};


#endif // ARRIVEANDALIGN_H