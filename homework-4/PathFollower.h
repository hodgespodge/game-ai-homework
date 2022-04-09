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

        GraphNode* start;
        GraphNode* goal;
        EuclideanHeuristic* heuristic;

        GraphNode* nextGlobalNode = NULL;


        sf::Vector2f globalTargetPosition;
        sf::Vector2f localTargetPosition;
        sf::Vector2f enemyPosition;

        std::vector<GraphNode*> graph;
        std::vector<room> rooms;

        std::list<GraphNode*> globalPath;
        std::list<sf::Vector2f> localPath;
        
        // sf::Vector2f localPathStart;
        // sf::Vector2f localPathEnd;

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

            start = new GraphNode(-1, 0, 0);
            goal = new GraphNode(-2, 0, 0);
            heuristic = new EuclideanHeuristic(goal);
            nextGlobalNode = NULL;

            locals = new ExposedVariables();

            tree = createTree(locals);

            // set Target Position to the current position
            globalTargetPosition = sf::Vector2f(rand() % 200, rand() % 200);
            needToUpdateGraph = true;
        }

        ~PathFollower(){
            delete start;
            delete goal;
            delete heuristic;
            delete locals;

        }

        sf::Vector2f findNearestPointOnLine(sf::Vector2f spritePos, sf::Vector2f lineStart, sf::Vector2f lineEnd){
            
            sf::Vector2f lineDir = lineEnd - lineStart;
            float lineLength = sqrt(pow(lineDir.x, 2) + pow(lineDir.y, 2));
            lineDir = lineDir / lineLength;

            float dotProduct = (spritePos.x - lineStart.x) * lineDir.x + (spritePos.y - lineStart.y) * lineDir.y;
            float nearestPointX = lineStart.x + lineDir.x * dotProduct;
            float nearestPointY = lineStart.y + lineDir.y * dotProduct;

            return sf::Vector2f(nearestPointX, nearestPointY);
        }

        // override the updateSprite function

        void updateSprite(Boid& sprite, float elapsedTime){

            sprite.updateBreadcrumbs();

            sf::Vector2f spritePos = sprite.getPosition();

            if (needToUpdateGraph) {
                updateGraph(sprite);
                needToUpdateGraph = false;
                std::vector<GraphNode*> globalPathVector = shortestPath(start, goal, graph, heuristic);

                globalPath = std::list<GraphNode*>(globalPathVector.rbegin(), globalPathVector.rend());

                if (globalPath.size() > 0) {
                    
                    // //print each node in global path
                    // for (GraphNode* node : globalPath) {
                    //     //print location
                    //     std::cout << "(" << node->x << ", " << node->y << ")" << std::endl;
                    // }


                    globalPath.pop_front();
                    nextGlobalNode = globalPath.front();


                    sf::Vector2f localPathStart = spritePos;
                    sf::Vector2f localPathEnd = sf::Vector2f(nextGlobalNode->x, nextGlobalNode->y);

                    // calculate point halfway between the path start and end
                    sf::Vector2f midwayPoint = localPathStart + (localPathEnd - localPathStart) / 2.0f;
                    room localRoom = getRoomFromCoordinates(rooms, midwayPoint.x, midwayPoint.y);

                    // Obstacles localObstacles = localRoom.obstacles;

                    std::vector<sf::Vector2f> localPathVector = getPathThroughRoom(localPathStart, localPathEnd, localRoom, localRoom.obstacles);
                    localPath = std::list<sf::Vector2f>(localPathVector.rbegin(), localPathVector.rend());

                    if (localPath.size() > 0) {
                        localPath.pop_front();
                        localTargetPosition = localPath.front();
        
                    }

                    std::cout << "local target position: " << localTargetPosition.x << ", " << localTargetPosition.y << std::endl;


                }else{
                    nextGlobalNode = NULL;
                }

            }

            if (locals->sprite == NULL){
                locals->sprite = &sprite;
            }

            if(update_number % 3 == 0){
                // update locals

                // locals-> sprite = &sprite;
                // locals-> local_target = localTargetPosition;
                // locals-> globalPath = globalPath;
                // locals-> localPath = localPath;
                // locals-> current_room = currentRoom;
                // locals-> enemy_position = enemyPosition;

                decision = tree->evaluate();

                std::cout << "decision: " << decision << std::endl;
            }

            switch(decision){
                case 0:
                    std::cout << "Get new path + target" << std::endl;

                    // Need to re-think this
                    //

                    break;
                case 1:
                    std::cout << "get next target" << std::endl;

                    if(localPath.size() > 0){
                        localPath.pop_front();
                        localTargetPosition = localPath.front();
                        locals->local_target = localTargetPosition;
                    }

                    break;
                case 2:
                    std::cout << "calculate new path" << std::endl;
                    break;
                case 3:
                    std::cout << "follow path" << std::endl;
                    break;
                
            }

            sprite.snapAngleToVelocity();

            sprite.updatePosition(elapsedTime);

            update_number++;

        }

        void updateGraph(Boid& sprite){
      
            graph = removeNodeFromGraph(graph,start);
            graph = removeNodeFromGraph(graph,goal);

            // if goal node and start node are in the same room, create 2 edges between them

            graph = cleanGraph(graph);

            start->x = sprite.getPosition().x;
            start->y = sprite.getPosition().y;

            goal->x = globalTargetPosition.x;
            goal->y = globalTargetPosition.y;

            // if goal node and start node are in the same room, create 2 edges between them
            room startRoom = getRoomFromCoordinates(rooms,start->x, start->y );
            room goalRoom = getRoomFromCoordinates(rooms,goal->x, goal->y );

            graph = addNodeToGraph(graph, rooms, start);
            graph = addNodeToGraph(graph, rooms, goal);
            
            // if goal node and start node are in the same room, create 2 edges between them
            if (getRoomFromCoordinates(rooms,start->x, start->y ).roomID == getRoomFromCoordinates(rooms,goal->x, goal->y ).roomID) {
                
                goal->addNeighbor(start, sqrt(pow(start->x - goal->x, 2) + pow(start->y - goal->y, 2)));
                start->addNeighbor(goal, sqrt(pow(start->x - goal->x, 2) + pow(start->y - goal->y, 2)));

            }

            heuristic->goal=goal;

        }

        void postUpdate(){

        }

        void checkEvent(sf::Event event){
            // Mouse click : set target position
            if (event.type == sf::Event::MouseButtonPressed)
            {

                globalTargetPosition = sf::Vector2f(sf::Mouse::getPosition(*window));
                needToUpdateGraph = true;

            }
        }

        std::string getName(){
            return "PathFollower";
        }


};

#endif