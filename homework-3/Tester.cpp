#include "Node.h"
#include <vector>
#include "Graph.h"
#include <iostream>


int main(int argc, char *argv[])
{

    // Read in a CSV list of edges and build a graph using Graph.h
    std::vector<Node*> graph = buildGraph("digraph.csv", "numNodes.csv");

    // Print out each node and the edges to each neighbor
    for(int i = 0; i < graph.size(); i++){
        // Print out the node
        std::cout << "Node " << graph[i]->id << ": ";
        std::cout << std::endl;

        // Print out the edges to each neighbor
        for(int j = 0; j < graph[i]->neighbors.size(); j++){
            // print out the cost and the neighbor
            std::cout << "--" << graph[i]->neighbors.top().first << "--> ( " << graph[i]->neighbors.top().second->id << " )";
            std::cout << std::endl;



            graph[i]->neighbors.pop();
        }

        std::cout << std::endl;
    }
}