#ifndef PATHFOLLOWER_H
#define PATHFOLLOWER_H

// subclass of steering behavior

#include "from-homework-2/SteeringBehavior.h"
#include "GraphNode.h"
#include "Graph.h"
#include "Heuristic.h"
#include "EuclideanHeuristic.h"
#include "SearchFunctions.h"

class PathFollower : public SteeringBehavior
{

    private:
        sf::RenderWindow* window;

        GraphNode* start;
        GraphNode* goal;
        EuclideanHeuristic* heuristic;

        GraphNode* nextNode;

        sf::Vector2f TargetPosition;
        std::vector<GraphNode*> graph;
        std::vector<room> rooms;
        std::vector<GraphNode*> path;
        
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
                std::vector<GraphNode*> path = shortestPath(-1, -2, graph, heuristic);
                // reverse the path
                std::reverse(path.begin(), path.end());

            }

            if(path.size() > 0){
                GraphNode* nextNode = path.at(0);
                path.erase(path.begin());

                // print next node location
                std::cout << "Next node: " << nextNode->x << " " << nextNode->y << std::endl;
            }

        }

        void updateGraph(Boid& sprite){
            // if target was already set, need to remove the old start and goal nodes
            
            graph = removeNodeFromGraph(graph,start);
            graph = removeNodeFromGraph(graph,goal);

            start->x = sprite.getPosition().x;
            start->y = sprite.getPosition().y;

            goal->x = TargetPosition.x;
            goal->y = TargetPosition.y;

            graph = addNodeToGraph(graph, rooms, start);
            graph = addNodeToGraph(graph, rooms, goal);

            graph = cleanGraph(graph);

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