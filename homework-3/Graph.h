#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Node.h"

// functions for building a graph of Nodes

// Read in a CSV list of edges
// Each line is of the form: "node1,node2,cost"
// where node1 and node2 are the ids of the nodes
// and cost is the cost of the edge
//
// Return an array of Nodes

std::vector<Node*> buildGraph(std::string edgesFile, int numNodes){
    std::vector<Node*> graph;

    // Initialize the graph with neighborless nodes
    for(int i = 0; i < numNodes; i++){
        graph.push_back(new Node(i));
    }

    // Read in the edges
    std::ifstream edgesFileStream(edgesFile);
    std::string line;

    // Read in each line as ints
    // The line is of the form: "node1,node2,cost"
    while(std::getline(edgesFileStream, line)){
        std::stringstream lineStream(line);
        std::string node1;
        std::string node2;
        std::string cost;
        std::getline(lineStream, node1, ',');
        std::getline(lineStream, node2, ',');
        std::getline(lineStream, cost, ',');
        int node1Int = std::stoi(node1);
        int node2Int = std::stoi(node2);
        int costInt = std::stoi(cost);

        // Add the edge to the graph
        graph[node1Int]->addNeighbor(graph[node2Int], costInt);
    }

    edgesFileStream.close();

    return graph;
}

std::vector<Node*> buildGraph(std::string edgesFile, std::string numNodesFile){
    // Read in the number of nodes
    std::ifstream numNodesFileStream(numNodesFile);
    int numNodes;
    numNodesFileStream >> numNodes;
    numNodesFileStream.close();

    // print out the number of nodes
    std::cout << "Number of nodes: " << numNodes << std::endl;
    
    return buildGraph(edgesFile, numNodes);

}


#endif
