#ifndef CATAI_H
#define CATAI_H

#include "SFML/Graphics.hpp"
#include <vector>
#include "from-homework-2/SteeringBehavior.h"
#include "from-homework-2/Boid.h"
#include "Room.h"
#include "GraphNode.h"

#include "from-homework-2/HelperFunctions.h"

#include "behaviorTree/EnemyBTree.h"


class CatAI 
{
    public:

        sf::RenderWindow * window;
        Boid * mouse;

        room * current_room;
        std::vector<room*> rooms;
        std::vector<GraphNode*> graph;
        bool is_initialized;
        float wander_speed;
        float chase_speed;

        BTNode * enemy_tree;
        std::unordered_map<int, ReturnType *> * blackboard;

        u_int32_t function_timer;

        room * wander_target;

        CatAI(sf::RenderWindow & window,std::vector<GraphNode*> graph, std::vector<room*> rooms, Boid & mouse)
        {
            this->window = &window;
            this->mouse = &mouse;
            this->graph = graph;
            this->rooms = rooms;

            current_room = rooms[0];
            is_initialized = false;
            wander_speed = 0.04;
            chase_speed = 0.06;
            function_timer = 0;

            blackboard = new std::unordered_map<int, ReturnType *>();

            ReturnType * findMouse = new ReturnType(ReturnType::IDLE);
            blackboard->insert({1, findMouse});
            ReturnType * chaseMouse = new ReturnType(ReturnType::IDLE);
            blackboard->insert({2, chaseMouse});
            ReturnType * dance = new ReturnType(ReturnType::IDLE);
            blackboard->insert({3, dance});
            ReturnType * wanderToRoom = new ReturnType(ReturnType::IDLE);
            blackboard->insert({4, wanderToRoom});
            
            enemy_tree = createTree(blackboard);

        }

        ~CatAI()
        {
            delete enemy_tree;
            delete blackboard;
        }

        void findMouse(Boid& sprite,  u_int32_t timeout){
            // find mouse
            
            sf::Vector2f currentPosition = sprite.getPosition();

            // check if mouse is in current room
            current_room = getRoomFromCoordinates(rooms, currentPosition.x, currentPosition.y);
            sf::Vector2f mousePosition = mouse->getPosition();

            if(current_room->contains(mousePosition.x, mousePosition.y)){
                // if mouse is in current room, success
                function_timer = 0;
                *blackboard->at(1) = ReturnType::SUCCESS;
            }

            else if(function_timer > timeout){
                function_timer = 0;
                *blackboard->at(1) = ReturnType::FAILURE;
            }

            else{
                // move towards mouse
                sf::Vector2f direction = mousePosition - currentPosition;
                direction = unitVector(direction);
                direction *= wander_speed;
                sprite.linearVelocity = direction;
            }
        }

        void chaseMouse(Boid& sprite, u_int32_t timeout){

            sf::Vector2f currentPosition = sprite.getPosition();
            sf::Vector2f mousePosition = mouse->getPosition();
            sf::Vector2f diff = mousePosition - currentPosition;


            if (magnitude(diff) < 50){
                // if mouse is close enough, success
                function_timer = 0;
                *blackboard->at(2) = ReturnType::SUCCESS;
            }

            else if(function_timer > timeout){
                function_timer = 0;
                *blackboard->at(2) = ReturnType::FAILURE;
            }

            else{
                // move towards mouse
                sf::Vector2f direction = mousePosition - currentPosition;
                direction = unitVector(direction);
                direction *= chase_speed;
                sprite.linearVelocity = direction;
            }
        }

        void dance(Boid& sprite, u_int32_t timeout){

            if(function_timer > timeout){
                function_timer = 0;
                *blackboard->at(3) = ReturnType::SUCCESS;
                return;
            }

            else{
                // spin around in a circle
                sf::Vector2f direction = sf::Vector2f(1,0);
                direction = unitVector(direction);
                direction *= wander_speed;
                sprite.linearVelocity = direction;
            }
        }

        void wanderToRoom(Boid& sprite, u_int32_t timeout){

            if(wander_target == nullptr){
                wander_target = rooms.at(rand() % rooms.size());
            }

            sf::Vector2f currentPosition = sprite.getPosition();

            current_room = getRoomFromCoordinates(rooms, currentPosition.x, currentPosition.y);
        
            if(current_room == wander_target){
                function_timer = 0;
                *blackboard->at(4) = ReturnType::SUCCESS;
                wander_target = nullptr;
                return;
            }

            else if(function_timer > timeout){
                function_timer = 0;
                *blackboard->at(4) = ReturnType::FAILURE;
                wander_target = nullptr;
                return;
            }

            sf::Vector2f mousePosition = mouse->getPosition();

            // if mouse in current room
            if(current_room->contains(mousePosition.x, mousePosition.y)){
                function_timer = 0;
                *blackboard->at(4) = ReturnType::FAILURE;
                wander_target = nullptr;
                return;

            }

            else{
                // move towards center of wander_target
                sf::Vector2f targetPosition = wander_target->getCenter();
                sf::Vector2f direction = targetPosition - currentPosition;
                direction = unitVector(direction);
                direction *= wander_speed;
                sprite.linearVelocity = direction;
                
            }
        }

        void resetTree(){
            for (auto value : *blackboard)
            {
                *value.second = IDLE;
            }
        }

        void updateSprite(Boid& sprite, float elapsedTime){
        
            function_timer += elapsedTime;

            std::pair<int, ReturnType> result = enemy_tree->evaluateNode();

            switch(result.second){

                case ReturnType::RUNNING:

                    switch(result.first){
                        case 1:
                            // std::cout << "findMouse" << std::endl;

                            findMouse(sprite, 7500);

                            break;
                        case 2:
                            // chase mouse

                            // std::cout << "chase mouse" << std::endl;

                            chaseMouse(sprite, 1000);

                            break;

                        case 3:
                            // dance

                            // std::cout << "dance" << std::endl;

                            dance(sprite, 5000);

                            break;
                        case 4:
                            // wander to room

                            // std::cout << "wander to room" << std::endl;

                            wanderToRoom(sprite, 10000);

                            break;
                        default:
                            // start from beginning

                            // std::cout << "start from beginning" << std::endl;
                            resetTree();
                            
                        }

                    break;

                case ReturnType::IDLE:
                    std::cout << "Error: got return type IDLE" << std::endl;
                    break;
                default:
                    // std::cout << "start from beginning" << std::endl;
                    resetTree();
            }

            
            sprite.updatePosition(elapsedTime);

        }

        void postUpdate();
        void checkEvent(sf::Event event);
        std::string getName(){
            return "CatAI";
        }

};

#endif