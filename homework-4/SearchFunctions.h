#ifndef SEARCHFUNCTIONS_H
#define SEARCHFUNCTIONS_H

#include "GraphNode.h"
#include <vector>
#include <functional>
#include <iostream>
#include "Heuristic.h"
#include "SFML/Graphics.hpp"

// Given the final SearchNode, reconstruct the path from the start node to the goal node using the parent pointers
// and return it as a vector of Node pointers.
std::vector<GraphNode*> reconstructPath(GraphNode* node){

    //Running reconstructPath on the goal node will return an empty vector

    std::vector<GraphNode*> path;
    while(node != NULL){
        path.push_back(node);
        node = node->parent;
    }
    return path;
}



// Perform an a* search on the graph from start to goal
// Return a vector of pointers to the nodes in the path

std::vector<GraphNode*> shortestPath(GraphNode* start, GraphNode* goal, std::vector<GraphNode*> nodeGraph, Heuristic* heuristic, sf::Vector2f enemy_pos, float enemy_radius){

    start->g = 0;
    start->h = heuristic->h(start);

    // Sort the open set by f(n) = g(n) + h(n)
    std::set<std::pair<float, GraphNode*>> open;
    
    // Add start node to the open set
    // First value is f(n)
    // Second value is the GraphNode itself
    open.insert(std::make_pair(start->f(), start));

    bool enemy_in_map = enemy_radius > 0;

    // While the open set is not empty
    while(!open.empty()){

        // Get the node with the lowest f(n)
        GraphNode* current = open.begin()->second;
        open.erase(open.begin());
        current->visited = true;
        
        // Check if we have reached the goal
        if(current->id == goal->id){
            return reconstructPath(current);
        }

        // For each neigbor in current->neighbors
        for(auto it = current->neighbors.begin(); it != current->neighbors.end(); ++it){
            GraphNode* neighbor = it->second;

            // If the neighbor has not been visited
            if(!neighbor->visited){
                // Calculate the cost of the path from start to neighbor
                float cost = current->g + it->first;

                if(enemy_in_map){
                    float enemy_dist = sqrt(pow(neighbor->x - enemy_pos.x, 2) + pow(neighbor->y - enemy_pos.y, 2));
                    
                    if (enemy_dist < enemy_radius){
                        cost += 1000;
                    }

                    cost += 2 * enemy_radius /(enemy_dist * enemy_dist * enemy_dist);
                }

                // If the cost is less than the neighbor's current cost
                if(cost < neighbor->g){
                    // Update the neighbor's cost
                    neighbor->g = cost;
                    neighbor->h = heuristic->h(neighbor);
                    neighbor->parent = current;

                    // If the neighbor is not in the open set
                    if(open.find(std::make_pair(neighbor->f(), neighbor)) == open.end()){
                        // Add the neighbor to the open set
                        open.insert(std::make_pair(neighbor->f(), neighbor));
                    }
                }
            }
        }


    }

    // If the open set is empty, there is no path
    return std::vector<GraphNode*>();
}

std::vector<GraphNode*> shortestPath(GraphNode* start, GraphNode* goal, std::vector<GraphNode*> nodeGraph, Heuristic* heuristic){

    sf::Vector2f placeholder_enemy = sf::Vector2f(-1000,-1000);
    float enemy_radius = 0;

    return shortestPath(start, goal, nodeGraph, heuristic, placeholder_enemy, enemy_radius);

}

std::vector<GraphNode*> shortestPath(int startID, int goalID, std::vector<GraphNode*> nodeGraph, Heuristic* heuristic){

    // Initialize the start node
    GraphNode* start = nodeGraph[startID];
    GraphNode* goal = nodeGraph[goalID];

    return shortestPath(start, goal, nodeGraph, heuristic);
}
    

#endif