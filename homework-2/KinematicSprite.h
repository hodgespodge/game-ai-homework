#ifndef KINEMATICSPRITE_H
#define KINEMATICSPRITE_H

#include <SFML/Graphics.hpp>

class KinematicSprite : public sf::Sprite

{
public:
    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    // Constructor
    KinematicSprite()
    {
        velocity = sf::Vector2f(0.0f, 0.0f);
        acceleration = sf::Vector2f(0.0f, 0.0f);
        // this->sprite = sprite;
    }

    // Destructor
    ~KinematicSprite()
    {
    }

    // update the sprite's velocity using its acceleration
    void updateVelocity(float elapsedTime)
    {
        velocity += acceleration * elapsedTime;
    }

    // update the sprite's position using its velocity
    void updatePosition(float elapsedTime)
    {
        // printf this sprite's current position
        // printf("%f, %f\n", getPosition().x, getPosition().y);
        
        move(velocity * elapsedTime);
    }

    // update the sprite's rotation to match its velocity
    void updateRotation()
    {
        if(velocity.x != 0 || velocity.y != 0){
            setRotation(atan2(velocity.y, velocity.x) * 180 / 3.14159265);
        }
    }


};

#endif