#ifndef SEARCHFUNCTIONS_H
#define SEARCHFUNCTIONS_H

#include "Node.h"
#include "SearchNode.h"
#include <vector>
#include <limits>
#include <functional>

const int INF = std::numeric_limits<int>::max();

// Given the final SearchNode, reconstruct the path from the start node to the goal node using the parent pointers
// and return it as a vector of Node pointers.
std::vector<Node*> reconstructPath(SearchNode* node){
    std::vector<Node*> path;
    while(node != NULL){
        path.push_back(node);
        node = node->parent;
    }
    return path;
}

// Perform an a* search on the graph from start to goal
// Return a vector of pointers to the nodes in the path
std::vector<Node*> shortestPath(Node* start, Node* goal, std::vector<Node*> graph, std::function<float (Node*, Node*)> heuristic){
    std::vector<Node*> path;

    // Sort the open set by f(n) = g(n) + h(n)
    std::set<std::pair<float, SearchNode*>> open;

    // g of start node is 0
    // h of start node is heuristic(start, goal)
    SearchNode* startNode = new SearchNode(start, 0, heuristic(start,goal));

    // Add start node to the open set
    open.insert(std::make_pair(startNode->f(), startNode));
    
}

#endif