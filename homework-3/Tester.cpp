#include "Node.h"
#include <vector>
#include "Graph.h"
#include <iostream>


int main(int argc, char *argv[])
{

    // Read in a CSV list of edges and build a graph using Graph.h
    std::vector<Node*> graph = buildGraph("graphFiles/digraph.csv", "graphFiles/numNodes.csv");



    // Print out each node and the edges to each neighbor
    for(int i = 0; i < graph.size(); i++){
        // Print out the node
        std::cout << "Node " << graph[i]->id << ": ";
        std::cout << std::endl;

        // For each neighbor, print out the neighbor and the cost of the edge
        for(auto neighbor : graph[i]->neighbors){
            std::cout << "    " << neighbor.second->id << ": " << neighbor.first << std::endl;
        }

        std::cout << std::endl;
    }

}