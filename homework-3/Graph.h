#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "Node.h"

// functions for building a graph of Nodes

// Read in a CSV list of edges
// Each line is of the form: "node1,node2,cost"
// where node1 and node2 are the ids of the nodes
// and cost is the cost of the edge
//
// Return a vector of Nodes
std::vector<Node*> buildGraph(std::string filename){
    std::vector<Node*> graph;
    std::ifstream file(filename);
    std::string line;
    std::string delimiter = ",";
    int node1, node2, cost;

    while(std::getline(file, line)){
        std::stringstream ss(line);
        std::string token;
        int i = 0;
        while(std::getline(ss, token, delimiter[0])){
            if(i == 0){
                node1 = std::stoi(token);
            }
            else if(i == 1){
                node2 = std::stoi(token);
            }
            else if(i == 2){
                cost = std::stoi(token);
            }
            i++;
        }

        Node* n1 = new Node(node1);
        Node* n2 = new Node(node2);
        n1->addNeighbor(n2, cost);
        n2->addNeighbor(n1, cost);
        graph.push_back(n1);
        graph.push_back(n2);
    }
    return graph;
}

#endif
