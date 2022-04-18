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

        Boid * enemy;


        std::vector<GraphNode*> graph;
        std::vector<room> rooms;

        // std::list<GraphNode*> globalPath;
        std::list<sf::Vector2f> globalPath;
        std::list<sf::Vector2f> localPath;
        

        bool needToUpdateGraph = false;

        room currentRoom;

        ExposedVariables * locals;
        DTNode * tree;

        u_short update_number = 0;
        bool is_initialized = false;

        int decision = 0;

        bool local_pathfinding_success = true;

        u_short max_local_target_update_number = 15;
        u_short local_target_update_number = 0;

        bool is_paused = false;

        u_int16_t max_pause_time = 25;
        u_int16_t pause_time = 0;

        sf::Vector2f previousLocalTarget;


    public:        
    
        int numBreadCrumbs = 10;
        int numBoids = 1;
        bool drawBreadcrumbs = true;
        bool fadeBreadcrumbs = true;
        bool drawID = false;
        // float pathOffset = 0.2;
        float maxSpeed = 0.05;
        float maxSpeedDelta = 0.02;

        float doorInnerRadius = 10;
        float doorOuterRadius = 100;
        sf::Sprite * cheese;
        
        PathFollower(sf::RenderWindow& window, std::vector<GraphNode*> graph, std::vector<room> rooms, sf::Sprite & cheese, Boid & enemy){
            this->window = &window;
            this->graph = graph;
            this->rooms = rooms;
            this->cheese = &cheese;
            this->enemy = &enemy;

            globalStart = new GraphNode(-1, 0, 0);
            globalGoal = new GraphNode(-2, 0, 0);
            heuristic = new EuclideanHeuristic(globalGoal);
            

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

            int wall_padding = 50;

            do {

                // get a random location in the room using x1, x2, y1, y2 with a padding
                x = randomRoom.x1 + wall_padding + (rand() % (randomRoom.x2 - randomRoom.x1 - wall_padding * 2)) ;
                y = randomRoom.y1 + wall_padding + (rand() % (randomRoom.y2 - randomRoom.y1 - wall_padding * 2)) ;

            
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

            globalPath.clear();

            // reverse for loop through the vector to get the path in the right order
            for (int i = globalPathVector.size() - 1; i >= 0; i--){
                globalPath.push_back(sf::Vector2f(globalPathVector[i]->x, globalPathVector[i]->y));
            }
            
            if (globalPath.size() > 0){
                globalNextTarget = globalPath.front();
                cheese->setPosition(globalPath.back());
                // make cheese no longer transparent
                cheese->setColor(sf::Color(255, 255, 255, 255));

            }
            else{
                globalNextTarget = sprite.getPosition();
            }

            localPath.clear();
            is_paused = true;
        

        }

        void getNewLocalPath(room localRoom, sf::Vector2f startPos, sf::Vector2f endPos){

            // if 

            if (!localRoom.contains_with_padding(startPos.x, startPos.y, 5)){
                sf::Vector2f starting_wall = localRoom.getWall(startPos.x, startPos.y, 5);

                if (starting_wall.x == -1 && starting_wall.y == -1){
                    // std::cout << "could not get starting wall for room" << std::endl;
                }else{

                    startPos = startPos + starting_wall*10.0f;

                }

            }

            sf::Vector2f enemyPosition = enemy->getPosition();


            std::vector<sf::Vector2f> localPathVector = getPathThroughRoom(startPos, endPos, localRoom, localRoom.obstacles, {enemyPosition.x, enemyPosition.y});

            localPath.splice(localPath.end(), std::list<sf::Vector2f>(localPathVector.rbegin(), localPathVector.rend()));

            if (localPathVector.size() != 0){
                localPath.emplace_back(endPos);
                local_pathfinding_success = true;
            }else{
                // std::cout << "No local path found between: " << startPos.x << "," << startPos.y << " and " << endPos.x << "," << endPos.y << std::endl;
                local_pathfinding_success = false;
            }

        }

        void followLocalPath(Boid& sprite, float elapsedTime){

            if (previousLocalTarget != localNextTarget){
                previousLocalTarget = localNextTarget;
                local_target_update_number = 0;
            } else{
                local_target_update_number++;

                // std::cout << "local target update number: " << local_target_update_number << std::endl;

                if (local_target_update_number > max_local_target_update_number){
                    localPath.clear();
                }
            }

            sf::Vector2f spritePos = sprite.getPosition();

            sf::Vector2f movementDirection;

            if(localPath.size() == 0){
                movementDirection = localNextTarget - spritePos;
            }else{
                movementDirection = (localNextTarget*0.7f + localPath.front()*0.3f)- spritePos;
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
        }

        void updateSprite(Boid& sprite, float elapsedTime){

            sprite.updateBreadcrumbs();

            if(! is_initialized){
                localNextTarget = sprite.getPosition();
                is_initialized = true;
            }

            if (needToUpdateGraph){
                getNewGlobalPath(sprite);
                needToUpdateGraph = false;
            }
            
            // update locals

            locals-> sprite = &sprite;
            locals-> local_target = localNextTarget;
            locals-> globalPath = globalPath;
            locals-> localPath = localPath;
            locals-> current_room = currentRoom;
            locals-> enemy_position = enemy->getPosition();
            locals-> is_paused = is_paused;

            decision = tree->evaluate();

            switch(decision){
                case 0:
                {

                    // std::cout << "case 0" << std::endl;

                    globalEndPosition = getRandomLocation();

                    getNewGlobalPath(sprite);

                    break;
                }

                case 1:
                {
                    

                    // if (!local_pathfinding_success){
                    //     // std::cout << "case 1b" << std::endl;
                        
                    //     globalPath.emplace_front(globalNextTarget);

                    //     sf::Vector2f midwayPoint = localNextTarget + (globalNextTarget - localNextTarget)/2.0f;
                    //     currentRoom = getRoomFromCoordinates(rooms,midwayPoint.x, midwayPoint.y);

                    //     // if (currentRoom.roomID == -1){
                    //     //     std::cout << " failed to find room in case 1b for coordinates: " << midwayPoint.x << "," << midwayPoint.y << std::endl;
                    //     // }

                    //     sf::Vector2f middleOfRoom = currentRoom.getCenter();

                    //     globalNextTarget = middleOfRoom;

                    //     getNewLocalPath(currentRoom, localNextTarget, globalNextTarget);

                    // }else{          
                        // std::cout << "case 1a" << std::endl;              
                        localNextTarget = globalNextTarget; // have the local target be the room's door

                        sf::Vector2f localPathStart = globalNextTarget; // for navigation, the start position is the room's door

                        globalNextTarget = globalPath.front();
                        globalPath.pop_front();

                        sf::Vector2f midwayPoint = localPathStart + (globalNextTarget - localPathStart) / 2.0f;
                        currentRoom = getRoomFromCoordinates(rooms, midwayPoint.x, midwayPoint.y);

                        // if (currentRoom.roomID == -1){
                        //     std::cout << " failed to find room in case 1a for coordinates: " << midwayPoint.x << "," << midwayPoint.y << std::endl;
                        // }

                        getNewLocalPath(currentRoom, localPathStart, globalNextTarget);

                    // }

                    break;
                }
                    
                case 2:
                {
                    // std::cout << "case 2" << std::endl;

                    localNextTarget = localPath.front();
                    localPath.pop_front();

                
                    break;
                }
                    
                case 3:
                {

                    // std::cout << "case 3" << std::endl;
                    sf::Vector2f localPathStart = sprite.getPosition();

                    localPath.clear();
                    

                    getNewLocalPath(currentRoom, localPathStart, globalNextTarget);


                    if (localPath.size() > 0){
                        localPath.pop_front();
                        localNextTarget = localPath.front();

                        followLocalPath(sprite, elapsedTime);
                    }

                    

                    break;
                }
                    
                case 4:
                {
                    // std::cout << "case 4" << std::endl;
                    
                    followLocalPath(sprite, elapsedTime);

                    break;
                }

                case 5:
                {
                    // std::cout << "case 5" << ": pos "<< sprite.getPosition().x << "," << sprite.getPosition().y << std::endl;
                    
                    if(pause_time > max_pause_time){
                        pause_time = 0;
                        is_paused = false;
                    }else{
                        pause_time += 1;
                        sprite.linearVelocity = sprite.linearVelocity * 0.9f;
                    }

                    break;
                }
                    
                default:
                {
                    followLocalPath(sprite, elapsedTime);

                    break; 
                }
                    
            }            

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

            // if (startRoom.roomID == -1){
                // std::cout << " failed to find room in updateGraph a for coords " << globalStart->x << "," << globalStart->y << std::endl;
            // }

            room goalRoom = getRoomFromCoordinates(rooms,globalGoal->x, globalGoal->y );

            // if (goalRoom.roomID == -1){
            //     std::cout << " failed to find room in updateGraph b for coords " << globalGoal->x << "," << globalGoal->y << std::endl;
            // }

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

                globalEndPosition = sf::Vector2f(sf::Mouse::getPosition(*window));
                
                needToUpdateGraph = true;
                localPath.clear();

            
            }
        }

        std::string getName(){
            return "PathFollower";
        }


};

#endif