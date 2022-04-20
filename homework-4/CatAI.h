#ifndef CATAI_H
#define CATAI_H

#include "SFML/Graphics.hpp"
#include <vector>
#include "from-homework-2/SteeringBehavior.h"
#include "from-homework-2/Boid.h"
#include "Room.h"
#include "GraphNode.h"


class CatAI 
{
    public:

        sf::RenderWindow * window;
        Boid * mouse;

        room * current_room;
        std::vector<room*> rooms;
        std::vector<GraphNode*> graph;
        bool is_initialized;
        float max_speed;

        CatAI(sf::RenderWindow & window,std::vector<GraphNode*> graph, std::vector<room*> rooms, Boid & mouse)
        {
            this->window = &window;
            this->mouse = &mouse;
            this->graph = graph;
            this->rooms = rooms;

            current_room = rooms[0];
            is_initialized = false;
            max_speed = 0.04;

        }

        ~CatAI()
        {
            
        }

        void updateSprite(Boid& sprite, float elapsedTime){
        
            sf::Vector2f currentPosition = sprite.getPosition();

            if(!is_initialized){
                current_room = getRoomFromCoordinates(rooms, currentPosition.x, currentPosition.y);
            }

            // move in the direction of the mouse 
            sf::Vector2f mousePosition = mouse->getPosition();
            sf::Vector2f direction = mousePosition - currentPosition;
            direction = unitVector(direction);
            direction *= max_speed;
            sprite.linearVelocity = direction;

            sprite.updatePosition(elapsedTime);

        }

        void postUpdate();
        void checkEvent(sf::Event event);
        std::string getName(){
            return "CatAI";
        }

};

#endif