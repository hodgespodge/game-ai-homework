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

        PathFollower(sf::RenderWindow& window, std::vector<GraphNode*> graph, std::vector<room> rooms){
            this->window = &window;
            this->graph = graph;
            this->rooms = rooms;

            start = new GraphNode(-1, 0, 0);
            goal = new GraphNode(-2, 0, 0);
            heuristic = new EuclideanHeuristic(goal);

        }

        // override the updateSprite function

        void updateSprite(Boid& sprite, float elapsedTime){

            if (needToUpdateGraph) {
                updateGraph(sprite);
                needToUpdateGraph = false;
                std::vector<GraphNode*> pathVector = shortestPath(start, goal, graph, heuristic);

                path = std::list<GraphNode*>(pathVector.rbegin(), pathVector.rend());

                // iterate through path and print
                // for (std::list<GraphNode*>::iterator it = path.begin(); it != path.end(); ++it) {
                //     std::cout << "(" << (*it)->x << ", " << (*it)->y << ")" << std::endl;
                // }

                if (path.size() > 0) {
                    nextNode = path.front();
                    // std::cout << "next node: " << nextNode->x << ", " << nextNode->y << std::endl;


                }else{
                    nextNode = NULL;
                }

            }

            // if sprite within 
            
            if (nextNode != NULL) {
                float distance = sqrt(pow(sprite.getPosition().x - nextNode->x, 2) + pow(sprite.getPosition().y - nextNode->y, 2));
                // std::cout << "distance: " << distance << std::endl;
                // std::cout << "path size: " << path.size() << std::endl;
                if (distance < 10 && path.size() > 0) {
                

                    path.pop_front();
                    nextNode = path.front();

                    std::cout << "next node: " << nextNode->x << ", " << nextNode->y << std::endl;
                    
                }
            }

            // move sprite towards next node
            if (nextNode != NULL) {

                sf::Vector2f direction = sf::Vector2f(nextNode->x - sprite.getPosition().x , nextNode->y - sprite.getPosition().y);
    
                if (magnitude(direction) != 0) {
                    sprite.linearVelocity = unitVector(direction) * 0.05f;
                    sprite.snapAngleToVelocity();
                }else{
                    sprite.linearVelocity = sf::Vector2f(0, 0);
                }

                sprite.updatePosition(elapsedTime);

            } else{
                sprite.linearVelocity = sf::Vector2f(0, 0);
            }

        }

        void updateGraph(Boid& sprite){
            // if target was already set, need to remove the old start and goal nodes
            
            graph = removeNodeFromGraph(graph,start);
            graph = removeNodeFromGraph(graph,goal);

            graph = cleanGraph(graph);

            start->x = sprite.getPosition().x;
            start->y = sprite.getPosition().y;

            // print the start node location
            // std::cout << "Start node: " << start->x << " " << start->y << std::endl;

            goal->x = TargetPosition.x;
            goal->y = TargetPosition.y;

            graph = addNodeToGraph(graph, rooms, start);
            graph = addNodeToGraph(graph, rooms, goal);

            heuristic->goal=goal;

            // // print the goal's neighboring nodes by printing the locations
            // for(auto neighbor : goal->neighbors){
            //     std::cout << "Goal Neighbor: " << neighbor.second->id << ": " << neighbor.second->x << ", " << neighbor.second->y << std::endl;

            // }

            // // print the start's neighboring nodes by printing the locations
            // for(auto neighbor : start->neighbors){
            //     std::cout << "Start Neighbor: " << neighbor.second->id << ": " << neighbor.second->x << ", " << neighbor.second->y << std::endl;

            // }

 

        }

        void postUpdate(){

        }

        void checkEvent(sf::Event event){
            // Mouse click : set target position
            if (event.type == sf::Event::MouseButtonPressed)
            {

                TargetPosition = sf::Vector2f(sf::Mouse::getPosition(*window));
                needToUpdateGraph = true;

                // std::cout << "Target position: " << TargetPosition.x << " " << TargetPosition.y << std::endl;
            }
        }

        std::string getName(){
            return "PathFollower";
        }


};

#endif