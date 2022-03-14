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

class PathFollower : public SteeringBehavior
{

    private:
        sf::RenderWindow* window;

        GraphNode* start;
        GraphNode* goal;
        EuclideanHeuristic* heuristic;

        GraphNode* nextNode = NULL;

        sf::Vector2f TargetPosition;
        std::vector<GraphNode*> graph;
        std::vector<room> rooms;
        std::list<GraphNode*> path;
        
        bool needToUpdateGraph = false;

    public:        
    
        int numBreadCrumbs = 10;
        int numBoids = 1;
        bool drawBreadcrumbs = true;
        bool fadeBreadcrumbs = true;
        bool drawID = false;
        float navSpeed = 0.2;

        PathFollower(sf::RenderWindow& window, std::vector<GraphNode*> graph, std::vector<room> rooms){
            this->window = &window;
            this->graph = graph;
            this->rooms = rooms;

            start = new GraphNode(-1, 0, 0);
            goal = new GraphNode(-2, 0, 0);
            heuristic = new EuclideanHeuristic(goal);
            nextNode = NULL;

        }

        ~PathFollower(){
            delete start;
            delete goal;
            delete heuristic;

        }

        // override the updateSprite function

        void updateSprite(Boid& sprite, float elapsedTime){

            sprite.updateBreadcrumbs();

            if (needToUpdateGraph) {
                updateGraph(sprite);
                needToUpdateGraph = false;
                std::vector<GraphNode*> pathVector = shortestPath(start, goal, graph, heuristic);

                path = std::list<GraphNode*>(pathVector.rbegin(), pathVector.rend());

                if (path.size() > 0) {
                    nextNode = path.front();

                }else{
                    nextNode = NULL;
                }

            }

            // if sprite within 
            if (nextNode != NULL) {
                float distance = sqrt(pow(sprite.getPosition().x - nextNode->x, 2) + pow(sprite.getPosition().y - nextNode->y, 2));
                if (distance < 10 && path.size() > 0) {
                

                    path.pop_front();
                    nextNode = path.front();

                }
            }

            // move sprite towards next node
            if (nextNode != NULL) {

                sf::Vector2f direction = sf::Vector2f(nextNode->x - sprite.getPosition().x , nextNode->y - sprite.getPosition().y);
    
                if (magnitude(direction) != 0) {
                    sprite.linearVelocity = unitVector(direction) * navSpeed;
                    sprite.snapAngleToVelocity();
                }else{
                    sprite.linearVelocity = sf::Vector2f(0, 0);
                }

            } else{
                sprite.linearVelocity = sf::Vector2f(0, 0);
            }

            sprite.updatePosition(elapsedTime);

        }

        void updateGraph(Boid& sprite){
      
            graph = removeNodeFromGraph(graph,start);
            graph = removeNodeFromGraph(graph,goal);

            // if goal node and start node are in the same room, create 2 edges between them

            graph = cleanGraph(graph);

            start->x = sprite.getPosition().x;
            start->y = sprite.getPosition().y;

            goal->x = TargetPosition.x;
            goal->y = TargetPosition.y;

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

                TargetPosition = sf::Vector2f(sf::Mouse::getPosition(*window));
                needToUpdateGraph = true;

            }
        }

        std::string getName(){
            return "PathFollower";
        }


};

#endif