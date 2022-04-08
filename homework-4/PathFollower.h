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
        std::vector<GraphNode*> graph;
        std::vector<room> rooms;

        std::list<GraphNode*> globalPath;
        std::list<sf::Vector2f> localPath;
        
        // sf::Vector2f localPathStart;
        // sf::Vector2f localPathEnd;

        bool needToUpdateGraph = false;

        room currentRoom;

        VariableMap * locals;
        DTNode * tree;

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

            locals = initLocals();
            tree = createTree(locals);

            // set Target Position to a random point on the screen
            globalTargetPosition = sf::Vector2f(rand() % window.getSize().x, rand() % window.getSize().y);
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

            // update locals
            locals->at("sprite") = sprite;

            sf::Vector2f spritePos = sprite.getPosition();            

            if (needToUpdateGraph) {
                updateGraph(sprite);
                needToUpdateGraph = false;
                std::vector<GraphNode*> globalPathVector = shortestPath(start, goal, graph, heuristic);

                globalPath = std::list<GraphNode*>(globalPathVector.rbegin(), globalPathVector.rend());

                // update locals
                locals->at("globalPath") = globalPath;

                if (globalPath.size() > 0) {
                    
                    nextGlobalNode = globalPath.front();

                    // update locals
                    // currentRoom = getRoomFromCoordinates( rooms, spritePos.x, spritePos.y);
                    // locals->at("current_room") = currentRoom;

                    // if (nextGlobalNode!= NULL) {

                    // 

                    sf::Vector2f localPathStart = spritePos;
                    sf::Vector2f localPathEnd = sf::Vector2f(nextGlobalNode->x, nextGlobalNode->y);

                    // calculate point halfway between the path start and end
                    sf::Vector2f midwayPoint = localPathStart + (localPathEnd - localPathStart) / 2.0f;
                    room localRoom = getRoomFromCoordinates(rooms, midwayPoint.x, midwayPoint.y);


                    localPath = getPathThroughRoom(localPathStart, localPathEnd, localRoom );

                    // update locals
                    locals->at("local_target") = localPathEnd;
                    // }

                }else{
                    nextGlobalNode = NULL;
                }

            }

            // update locals
            // locals->at("local_target") = localPathEnd;
            locals->at("local_path") = globalPath;
            locals->at("enemy_position") = sf::Vector2f(25,100);

            int decision = tree->evaluate();

            std::cout << "decision: " << decision << std::endl;

            // if (nextGlobalNode != NULL) {
            //     float distance = sqrt(pow(sprite.getPosition().x - nextGlobalNode->x, 2) + pow(sprite.getPosition().y - nextGlobalNode->y, 2));
            //     if (distance < doorInnerRadius && globalPath.size() > 0) {
                

            //         globalPath.pop_front();
            //         nextGlobalNode = globalPath.front();

            //         if (nextGlobalNode!= NULL) {
            //             localPathStart = sprite.getPosition();
            //             localPathEnd = sf::Vector2f(nextGlobalNode->x, nextGlobalNode->y);
            //         }


            //     } else if (distance < doorOuterRadius) {
            //         // reduce velocity distance to door;
            //         sprite.linearVelocity = sprite.linearVelocity * 0.95f;
            //     }
            // }

            // // move sprite along the straight line globalPath defined by localPathStart and localPathEnd
            // if (nextGlobalNode != NULL) {
                
            //     // find the nearest point on the straight line from the sprite
            //     sf::Vector2f nearestPoint = findNearestPointOnLine(sprite.getPosition(), localPathStart, localPathEnd);

            //     // get a point on the straight line that is pathOffset units away from the nearest point towards the localPathEnd
            //     sf::Vector2f targetPoint = sf::Vector2f(nearestPoint.x + (localPathEnd.x - nearestPoint.x) * pathOffset, nearestPoint.y + (localPathEnd.y - nearestPoint.y) * pathOffset);

            //     // get the direction of the target point from the sprite's position
            //     sf::Vector2f direction = targetPoint - sprite.getPosition();

            //     // normalize the direction vector
            //     float length = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
            //     direction = direction / length;

            //     // update the sprite's velocity to move it towards the target point
            //     sprite.linearVelocity += direction * maxSpeedDelta;

            //     // if the sprite's velocity is greater than the max speed, normalize the velocity to the max speed
            //     if(magnitude(sprite.linearVelocity) > maxSpeed){
            //         sprite.linearVelocity = sprite.linearVelocity / magnitude(sprite.linearVelocity) * maxSpeed;
            //     }


            // } else{

            //     if(magnitude(sprite.linearVelocity) > 0.01){
            //         sprite.linearVelocity = sprite.linearVelocity / magnitude(sprite.linearVelocity) * maxSpeedDelta;
            //     } else{
            //         sprite.linearVelocity = sf::Vector2f(0, 0);
            //     }
                
            // }

            sprite.snapAngleToVelocity();

            sprite.updatePosition(elapsedTime);

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