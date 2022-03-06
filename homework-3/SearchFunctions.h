#ifndef SEARCHFUNCTIONS_H
#define SEARCHFUNCTIONS_H

#include "GraphNode.h"
#include <vector>
#include <functional>
#include <iostream>

// Given the final SearchNode, reconstruct the path from the start node to the goal node using the parent pointers
// and return it as a vector of Node pointers.
std::vector<GraphNode*> reconstructPath(GraphNode* node){
    std::vector<GraphNode*> path;
    while(node != NULL){
        path.push_back(node);
        node = node->parent;
    }
    return path;
}

// Perform an a* search on the graph from start to goal
// Return a vector of pointers to the nodes in the path
std::vector<GraphNode*> shortestPath(int startID, int goalID, std::vector<GraphNode*> nodeGraph, std::function<float (GraphNode*, GraphNode*)> heuristic){

    // Initialize the start node
    GraphNode* start = nodeGraph[startID];
    GraphNode* goal = nodeGraph[goalID];

    start->g = 0;
    start->h = heuristic(start, goal);

    // Sort the open set by f(n) = g(n) + h(n)
    std::set<std::pair<float, GraphNode*>> open;
    
    // Add start node to the open set
    // First value is f(n)
    // Second value is the SearchNode itself
    open.insert(std::make_pair(start->f(), start));

    // While the open set is not empty
    while(!open.empty()){

        // Get the node with the lowest f(n)
        GraphNode* current = open.begin()->second;
        open.erase(open.begin());
        current->visited = true;

        // Check if we have reached the goal
        if(current->id == goalID){
            return reconstructPath(current);
        }

        // neighbors of current node ordered by path cost from current node to neighbor
        std::set<std::pair<int, GraphNode*>> neighbors = current->neighbors;


    }

    // If the open set is empty, there is no path
    return std::vector<GraphNode*>();
}

#endif