#ifndef ARRIVEANDALIGN_H
#define ARRIVEANDALIGN_H

#include <SFML/Graphics.hpp>
// #include "KinematicSprite.h"
#include "SteeringBehavior.h"

class ArriveAndAlign : public SteeringBehavior
{
private:
    sf::RenderWindow* window;
    sf::Vector2i TargetPosition;
public:
    // Constructor
    ArriveAndAlign(sf::RenderWindow& window)
    {
        this->window = &window;
    }

    void updateSprite(KinematicSprite& sprite, float elapsedTime)
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