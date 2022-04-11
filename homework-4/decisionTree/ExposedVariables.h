#ifndef EXPOSEDVARIABLES_H
#define EXPOSEDVARIABLES_H

#include "SFML/Graphics.hpp"
#include <vector>
#include "../GraphNode.h"
#include "../Room.h"
#include "../from-homework-2/Boid.h"

class ExposedVariables 
{
public:

    Boid * sprite;
    sf::Vector2f local_target;
    std::list<sf::Vector2f> globalPath;
    std::list<sf::Vector2f> localPath;
    room current_room;
    sf::Vector2f enemy_position;

    // Constructor
    ExposedVariables()
    {
        this->sprite = NULL;
        this->local_target = sf::Vector2f(0,0);
        this->globalPath = std::list<sf::Vector2f>();
        this->localPath = std::list<sf::Vector2f>();
        this->current_room = room();
        this->enemy_position = sf::Vector2f(0,0);
        
    }

};

#endif