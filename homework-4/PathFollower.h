#ifndef PATHFOLLOWER_H
#define PATHFOLLOWER_H

// subclass of steering behavior

#include "from-homework-2/SteeringBehavior.h"
#include "from-homework-2/HelperFunctions.h"
#include "GraphNode.h"
#include "Graph.h"
#include "Heuristic.h"
#include "EuclideanHeuristic.h"
#include "SearchFunctions.h"
#include <list>
#include "decisionTree/LocalNavTree.h"
#include "decisionTree/ExposedVariables.h"
#include "LocalNavFunctions.h"

class PathFollower : public SteeringBehavior
{

    private:
        sf::RenderWindow* window;

        GraphNode* globalStart;
        GraphNode* globalGoal;
        EuclideanHeuristic* heuristic;

        sf::Vector2f globalEndPosition; // The end goal in the global graph

        sf::Vector2f globalNextTarget; // The next position (door) in the global graph being navigated to
        sf::Vector2f localNextTarget; // The next position (coord) in the local graph being navigated to

        sf::Vector2f enemyPosition;

        std::vector<GraphNode*> graph;
        std::vector<room> rooms;

        std::list<GraphNode*> globalPath;
        std::list<sf::Vector2f> localPath;
        

        bool needToUpdateGraph = false;

        room currentRoom;

        ExposedVariables * locals;
        DTNode * tree;

        u_short update_number = 0;

        int decision = 0;


    public:        
    
        int numBreadCrumbs = 10;
        int numBoids = 1;
        bool drawBreadcrumbs = true;
        bool fadeBreadcrumbs = true;
        bool drawID = false;
        float pathOffset = 0.2;
        float maxSpeed = 0.1;
        float maxSpeedDelta = 0.004;

        float doorInnerRadius = 10;
        float doorOuterRadius = 100;
        
        

        PathFollower(sf::RenderWindow& window, std::vector<GraphNode*> graph, std::vector<room> rooms){
            this->window = &window;
            this->graph = graph;
            this->rooms = rooms;

            globalStart = new GraphNode(-1, 0, 0);
            globalGoal = new GraphNode(-2, 0, 0);
            heuristic = new EuclideanHeuristic(globalGoal);
            // nextGlobalNode = NULL;

            locals = new ExposedVariables();

            tree = createTree(locals);

            // set Target Position to the current position
            globalEndPosition = getRandomLocation();
            needToUpdateGraph = true;
            enemyPosition = sf::Vector2f(11111, 11111);
        }

        ~PathFollower(){
            delete globalStart;
            delete globalGoal;
            delete heuristic;
            delete locals;

        }

        sf::Vector2f getRandomLocation(){

            // get a random room
            room randomRoom = rooms[rand() % rooms.size()];

            // get a random location in the room using x1, x2, y1, y2 

            float x = 0;
            float y = 0;

            bool no_collision = false;

            do {
                x = randomRoom.x1 + (rand() % (randomRoom.x2 - randomRoom.x1));
                y = randomRoom.y1 + (rand() % (randomRoom.y2 - randomRoom.y1));
            
                no_collision = true;

                // loop through the obstacles in the room and check if the random location is inside any of them
                for (auto obstacle : randomRoom.obstacles){
                    if (sqrt(pow(x - obstacle.first.x, 2) + pow(y - obstacle.first.y, 2)) < obstacle.second){
                        no_collision = false;
                        break;
                    }
                }

            } while(!no_collision);

            return sf::Vector2f(x, y);
        }

        void getNewGlobalPath(Boid & sprite){

            updateGraph(sprite);

            std::vector<GraphNode*> globalPathVector = shortestPath(globalStart, globalGoal, graph, heuristic);
            globalPath = std::list<GraphNode*>(globalPathVector.rbegin(), globalPathVector.rend());

            localNextTarget = sprite.getPosition();
        }

        void getNewLocalPath(room localRoom, sf::Vector2f startPos, sf::Vector2f endPos){
            std::vector<sf::Vector2f> localPathVector = getPathThroughRoom(startPos, endPos, localRoom, localRoom.obstacles);
            localPath = std::list<sf::Vector2f>(localPathVector.rbegin(), localPathVector.rend());

            if (localPath.size() == 0){

                globalEndPosition = getRandomLocation();
                needToUpdateGraph = true;
            }
        }

        void updateSprite(Boid& sprite, float elapsedTime){

            sprite.updateBreadcrumbs();

            if(update_number == 0){
                localNextTarget = sprite.getPosition();
            }

            if(needToUpdateGraph){
                getNewGlobalPath(sprite);
                needToUpdateGraph = false;
            }

            if(update_number % 1 == 0){
                // update locals

                locals-> sprite = &sprite;
                locals-> local_target = localNextTarget;
                locals-> globalPath = globalPath;
                locals-> localPath = localPath;
                locals-> current_room = currentRoom;
                locals-> enemy_position = enemyPosition;

                decision = tree->evaluate();

                // std::cout << "decision: " << decision << std::endl;
            }

            switch(decision){
                case 0:
                {
                    // std::cout << "Get new global target and a new global path" << std::endl;

                    // globalEndPosition = sf::Vector2f(rand() % 500, rand() % 500);
                    globalEndPosition = getRandomLocation();

                    getNewGlobalPath(sprite);

                    break;
                }

                case 1:
                {
                    sf::Vector2f localPathStart = globalNextTarget; // set the local path start to the door of the room we are about to enter
                    localNextTarget = localPathStart;

                    GraphNode* nextGlobalNode = globalPath.front();
                    globalPath.pop_front();

                    globalNextTarget = sf::Vector2f(nextGlobalNode->x, nextGlobalNode->y);

                    sf::Vector2f midwayPoint = localPathStart + (globalNextTarget - localPathStart) / 2.0f;
                    currentRoom = getRoomFromCoordinates(rooms, midwayPoint.x, midwayPoint.y);

                    getNewLocalPath(currentRoom, localPathStart, globalNextTarget);

                    // print each obstacle in the room as well as the radius
                    for (auto obstacle : currentRoom.obstacles){
                        std::cout << "(" << obstacle.first.x << ", " << obstacle.first.y << ") " << obstacle.second << std::endl;
                    }

                    break;
                }
                    
                case 2:
                {
                    // std::cout << "pop local path + get next local node" << std::endl;

                    localPath.pop_front();
                    localNextTarget = localPath.front();

                    break;
                }
                    
                case 3:
                {
                    // std::cout << "get new local path" << std::endl;

                    sf::Vector2f localPathStart = sprite.getPosition();

                    getNewLocalPath(currentRoom, localPathStart, globalNextTarget);

                    break;
                }
                    
                case 4:
                {
                    sf::Vector2f spritePos = sprite.getPosition();
                    sf::Vector2f localOffset = localNextTarget - spritePos;
                    
                    if (magnitude(localOffset) >  0.0f){
                        sprite.linearVelocity = unitVector(localOffset)*0.05f;
                    }else {
                        sprite.linearVelocity = sf::Vector2f(0,0);
                    }


                    break;
                }
                    
                default:
                {
                //    std::cout << "default: follow path" << std::endl;

                    sf::Vector2f spritePos = sprite.getPosition();
                    sf::Vector2f localOffset = localNextTarget - spritePos;
                    
                    if (magnitude(localOffset) >  0.0f){
                        sprite.linearVelocity = unitVector(localOffset)*0.1f;
                    }else {
                        sprite.linearVelocity = sf::Vector2f(0,0);
                    }


                    break; 
                }
                    
            }            
            
            // std::cout << "Next local target: " << localNextTarget.x << " " << localNextTarget.y << std::endl;
            // for (auto it = localPath.begin(); it != localPath.end(); ++it){
            //     std::cout << "local path: " << it->x << " " << it->y << std::endl;
            // }

            // std::cout << "Next global target: " << globalNextTarget.x << " " << globalNextTarget.y << std::endl;
            
            // // iterate through the global path list and print
            // for (auto it = globalPath.begin(); it != globalPath.end(); ++it){
            //     std::cout << "global path: " << (*it)->x << " " << (*it)->y << std::endl;
            // }


            decision = -1;

            sprite.snapAngleToVelocity();

            sprite.updatePosition(elapsedTime);

            update_number++;

        }

        void updateGraph(Boid& sprite){
      
            graph = removeNodeFromGraph(graph,globalStart);
            graph = removeNodeFromGraph(graph,globalGoal);

            // if globalGoal node and globalStart node are in the same room, create 2 edges between them

            graph = cleanGraph(graph);

            globalStart->x = sprite.getPosition().x;
            globalStart->y = sprite.getPosition().y;

            globalGoal->x = globalEndPosition.x;
            globalGoal->y = globalEndPosition.y;

            // if globalGoal node and globalStart node are in the same room, create 2 edges between them
            room startRoom = getRoomFromCoordinates(rooms,globalStart->x, globalStart->y );
            room goalRoom = getRoomFromCoordinates(rooms,globalGoal->x, globalGoal->y );

            graph = addNodeToGraph(graph, rooms, globalStart);
            graph = addNodeToGraph(graph, rooms, globalGoal);
            
            // if globalGoal node and globalStart node are in the same room, create 2 edges between them
            if (getRoomFromCoordinates(rooms,globalStart->x, globalStart->y ).roomID == getRoomFromCoordinates(rooms,globalGoal->x, globalGoal->y ).roomID) {
                
                globalGoal->addNeighbor(globalStart, sqrt(pow(globalStart->x - globalGoal->x, 2) + pow(globalStart->y - globalGoal->y, 2)));
                globalStart->addNeighbor(globalGoal, sqrt(pow(globalStart->x - globalGoal->x, 2) + pow(globalStart->y - globalGoal->y, 2)));

            }

            heuristic->goal=globalGoal;

        }

        void postUpdate(){

        }

        void checkEvent(sf::Event event){
            // Mouse click : set target position
            if (event.type == sf::Event::MouseButtonPressed)
            {

                // globalEndPosition = sf::Vector2f(sf::Mouse::getPosition(*window));
                
                // needToUpdateGraph = true;

                std::cout << "Mouse click: " << sf::Mouse::getPosition(*window).x << " " << sf::Mouse::getPosition(*window).y << std::endl;

            }
        }

        std::string getName(){
            return "PathFollower";
        }


};

#endif