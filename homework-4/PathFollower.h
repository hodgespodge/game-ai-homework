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
#include "decisionTree/NavTree.h"
#include "decisionTree/ExposedVariables.h"
// #include "LocalNavFunctions.h"

class PathFollower : public SteeringBehavior
{

    private:
        sf::RenderWindow* window;

        GraphNode* startNode;
        GraphNode* goalNode;
        EuclideanHeuristic* heuristic;

        sf::Vector2f globalTarget;

        sf::Vector2f localTarget; // The next position (coord) in the local graph being navigated to


        Boid * enemy;


        std::vector<GraphNode*> graph;
        std::vector<room*>  rooms;

        std::list<sf::Vector2f> path;
        
        std::list<sf::Vector2f> targetQueue;

        room * currentRoom;

        ExposedVariables * locals;
        DTNode * tree;

        u_int64_t update_number = 0;
        bool is_initialized = false;

        int decision = 0;

        bool local_pathfinding_success = true;

        // u_short max_local_target_update_number = 15;
        // u_short local_target_update_number = 0;

        bool is_paused = false;

        u_int16_t max_pause_time = 500;
        u_int16_t pause_time = 0;

        sf::Vector2f previousLocalTarget;

        float target_range = 10;


    public:        
    
        int numBreadCrumbs = 10;
        int numBoids = 1;
        bool drawBreadcrumbs = true;
        bool fadeBreadcrumbs = true;
        bool drawID = false;
        // float pathOffset = 0.2;
        float maxSpeed = 0.05;
        float maxSpeedDelta = 0.02;

        // float doorInnerRadius = 10;
        // float doorOuterRadius = 100;
        sf::Sprite * cheese;

        bool needToUpdateGraph = true;

        // bool shiftPressed = false;
        
        PathFollower(sf::RenderWindow& window, std::vector<GraphNode*> graph, std::vector<room*> rooms, sf::Sprite & cheese, Boid & enemy){
            this->window = &window;
            this->graph = graph;
            this->rooms = rooms;
            this->cheese = &cheese;
            this->enemy = &enemy;

            startNode = new GraphNode(-1, 0, 0);
            goalNode = new GraphNode(-2, 0, 0);
            heuristic = new EuclideanHeuristic(goalNode);
            

            locals = new ExposedVariables();

            tree = createTree(locals);

            // targetQueue.push_back(getRandomLocation());
            // cheese.setPosition(targetQueue.back());

        }

        ~PathFollower(){
            delete startNode;
            delete goalNode;
            delete heuristic;
            delete locals;

        }

        sf::Vector2f getRandomLocation(){

            // get a random room
            // room randomRoom = rooms[rand() % rooms.size()];
            room * randomRoom = rooms[rand() % rooms.size()];

            // get a random location in the room using x1, x2, y1, y2 

            float x = 0;
            float y = 0;

            bool no_collision = false;

            int wall_padding = 50;

            do {

                // get a random location in the room using x1, x2, y1, y2 with a padding
                x = randomRoom->x1 + wall_padding + (rand() % (randomRoom->x2 - randomRoom->x1 - wall_padding * 2)) ;
                y = randomRoom->y1 + wall_padding + (rand() % (randomRoom->y2 - randomRoom->y1 - wall_padding * 2)) ;

            
                no_collision = true;

                // loop through the obstacles in the room and check if the random location is inside any of them
                for (auto obstacle : randomRoom->obstacles){
                    if (sqrt(pow(x - obstacle.first.x, 2) + pow(y - obstacle.first.y, 2)) < obstacle.second){
                        no_collision = false;
                        break;
                    }
                }

            } while(!no_collision);

            // std::cout << "Random Location: " << x << ", " << y << std::endl;

            return sf::Vector2f(x, y);
        }

        void getNewPath(Boid & sprite){
            
            updateGraph(sprite);

            std::vector<GraphNode*> pathVector = shortestPath(startNode, goalNode, graph, heuristic, enemy->getPosition(), 50);

            path.clear();

            // reverse for loop through the vector to get the path in the right order
            for (int i = pathVector.size() - 1; i >= 0; i--){
                path.push_back(sf::Vector2f(pathVector[i]->x, pathVector[i]->y));
            }
            
            if (path.size() > 0){
                localTarget = path.front();
                path.pop_front();

                cheese->setPosition(path.back());
                // make cheese no longer transparent
                cheese->setColor(sf::Color(255, 255, 255, 255));

            }
            else{
                // std::cout << "No path found" << std::endl;
                localTarget = sprite.getPosition();
            }

        }

        void followLocalPath(Boid& sprite, float elapsedTime){

            sf::Vector2f spritePos = sprite.getPosition();

            sf::Vector2f movementDirection;
            
            // movementDirection = localTarget - spritePos;

            if(path.size() == 0){ 
                movementDirection = localTarget - spritePos;
            }else{
                movementDirection = (localTarget*0.8f + path.front()*0.2f)- spritePos;
            }

            if (magnitude(movementDirection) >  0.0f){

                // increase the velocity of the sprite by max velocity change in the direction of the movement
                sprite.linearVelocity += unitVector(movementDirection)*maxSpeedDelta;

                // if the velocity is greater than the max velocity, set it to the max velocity
                if (magnitude(sprite.linearVelocity) > maxSpeed){
                    sprite.linearVelocity = unitVector(sprite.linearVelocity)*maxSpeed;
                }
                
            }else {
                sprite.linearVelocity = sf::Vector2f(0,0);
            }

            sprite.updatePosition(elapsedTime);
            sprite.snapAngleToVelocity();
        }

        void updateSprite(Boid& sprite, float elapsedTime){

            sprite.updateBreadcrumbs();

            if (update_number == 0){
                localTarget = sprite.getPosition();
            }

            // if (needToUpdateGraph){
            //     updateGraph(sprite);
            // }

            room * currentRoom = getRoomFromCoordinates(rooms, localTarget.x, localTarget.y);

            sf::Vector2f spritePos = sprite.getPosition();
            sf::Vector2f enemyPos = enemy->getPosition();
            
            // if current room contains the cat

            if (currentRoom->contains(enemyPos.x, enemyPos.y)){
        
                if (sqrt(pow(spritePos.x - enemy->getPosition().x, 2) + pow(spritePos.y - enemy->getPosition().y, 2)) < 20){
                    path.clear();

                    // teleport the sprite to a random location

                    is_paused = true;

                    localTarget = sprite.getPosition();

                }
            }

            locals-> sprite = &sprite;
            locals-> local_target = localTarget;
            locals-> targetQueue = targetQueue;
            locals-> localPath = path;
            locals-> current_room = currentRoom;
            locals-> enemy_position = enemy->getPosition();
            locals-> is_paused = is_paused;

            decision = tree->evaluate(update_number);

            switch(decision){
                case 0:
                {

                    this->targetQueue.push_back(getRandomLocation());

                    break;
                }

                case 1:
                {
                    
                    this->globalTarget = targetQueue.front();
                    targetQueue.pop_front();

                    this->localTarget = globalTarget;
                    path.clear();

                    cheese->setPosition(globalTarget);
                    
                    break;
                }
                    
                case 2:
                {

                    do {

                        this->localTarget = path.front();
                        path.pop_front();

                    } while( path.size() > 0 && sqrt(pow(sprite.getPosition().x - localTarget.x, 2) + pow(sprite.getPosition().y - localTarget.y, 2)) < target_range);

                    followLocalPath(sprite, elapsedTime);

                    break;
                }
                    
                case 3:
                {

                    getNewPath(sprite);

                    followLocalPath(sprite, elapsedTime);

                    break;
                }
                    
                case 4:
                {

                    break;
                }

                case 5:
                {

                    if(pause_time > max_pause_time){
                        pause_time = 0;
                        is_paused = false;
                        sprite.angularVelocity = 0;
                        sprite.setPosition(getRandomLocation());
                    }else{
                        pause_time += elapsedTime;
                        sprite.angularVelocity += 0.2f;
                        sprite.linearVelocity = sprite.linearVelocity * 0.92f;

                        sprite.updatePosition(elapsedTime);
                        sprite.updateAngle(elapsedTime);
                    }

                    break;
                }

                case 6:
                {

                    // loop through the next 10 nodes in the path and check if any of them are in the enemy
                    maxSpeed = 0.07;
                    maxSpeedDelta = 0.03;

                    // target_range = 20;

                    // path.clear();
                    getNewPath(sprite);

                    while( path.size() > 0 && sqrt(pow(sprite.getPosition().x - localTarget.x, 2) + pow(sprite.getPosition().y - localTarget.y, 2)) < target_range * 2){
                        this->localTarget = path.front();
                        path.pop_front();
                    }

                    followLocalPath(sprite, elapsedTime);

                    break;
                }
                    
                case 7:
                {
                    // std::cout << "-follow path normally" << std::endl;

                    maxSpeed = 0.05;
                    maxSpeedDelta = 0.02;

                    // target_range = 10;

                    followLocalPath(sprite, elapsedTime);

                    break;
                }

                default:
                {
                    followLocalPath(sprite, elapsedTime);

                    break; 
                }
                    
            }            

            // sprite.snapAngleToVelocity();

            // sprite.updatePosition(elapsedTime);

            update_number++;

        }

        void updateGraph(Boid& sprite){
      
            graph = removeNodeFromGraph(graph,startNode);
            graph = removeNodeFromGraph(graph,goalNode);

            graph = cleanGraph(graph);

            startNode->x = sprite.getPosition().x;
            startNode->y = sprite.getPosition().y;

            goalNode->x = globalTarget.x;
            goalNode->y = globalTarget.y;

            graph = addNodeToGraph(graph, rooms, startNode);
            graph = addNodeToGraph(graph, rooms, goalNode);

            heuristic->goal=goalNode;

        }

        void postUpdate(){

        }

        void checkEvent(sf::Event event){
            // Mouse click : set target position
            
            if (event.type == sf::Event::MouseButtonPressed)
            {

                if (!shiftPressed){
                    targetQueue.clear();
                    path.clear();

                    needToUpdateGraph = true;
                    globalTarget = sf::Vector2f(sf::Mouse::getPosition(*window));
                
                }else{

                    targetQueue.push_back(sf::Vector2f(sf::Mouse::getPosition(*window)));
                }
      

            }
        }

        std::string getName(){
            return "PathFollower";
        }


};

#endif