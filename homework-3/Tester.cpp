#include "GraphNode.h"
#include <vector>
#include "Graph.h"
#include <iostream>
#include "SearchFunctions.h"
#include <functional>
#include <string>
#include <sys/stat.h>

float heuristic(GraphNode* start, GraphNode* goal){
    return 0;
}

int main(int argc, char *argv[])
{

    // string = filename
    std::string graphFile = "graphFiles/digraph.csv";
    std::string numNodesFile = "graphFiles/numNodes.csv";

    // if "-n" or "--new" is passed, set new graph boolean to true
    bool newGraph = false;
    if(argc > 1){
        if(std::string(argv[1]) == "-n" || std::string(argv[1]) == "--new"){
            newGraph = true;
        }
    }

    // check using stat to see if the files exist
    struct stat buffer;
    if (newGraph || stat(graphFile.c_str(), &buffer) != 0 || stat(numNodesFile.c_str(), &buffer) != 0) {
        
        // Call randomDigraph.py to generate the files
        std::string command = "python3 scripts/randomDigraph.py";
        system(command.c_str());

    }

    // Read in a CSV list of edges and build a graph using Graph.h
    std::vector<GraphNode*> graph = buildGraph(graphFile, numNodesFile);

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

    // Call the a* search function
    std::vector<GraphNode*> path = shortestPath(0, graph.size()-1, graph, heuristic);

    std::cout << "Path: ";

    // Print out the path
    for(auto node : path){
        std::cout << node->id << " ";
    }

    std::cout << std::endl;

}