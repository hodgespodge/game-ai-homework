#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>

#include "GraphNode.h"

// functions for building a graph of Nodes

// Read in a CSV list of edges
// Each line is of the form: "node1,node2,cost"
// where node1 and node2 are the ids of the nodes
// and cost is the cost of the edge
//
// Return an array of Nodes

std::vector<GraphNode*> buildGraph(std::string edgesFile, std::string nodesFile ){
    std::vector<GraphNode*> graph;

    // Read in each line of the nodes file as an int
    // The line is of the form: "nodeID, x, y"
    std::ifstream nodesFileStream(nodesFile);
    std::string line;
    while (std::getline(nodesFileStream, line))
    {
        std::stringstream lineStream(line);
        std::string node;
        std::string x;
        std::string y;

        std::getline(lineStream, node, ',');
        std::getline(lineStream, x, ',');
        std::getline(lineStream, y, ',');

        int id = std::stoi(node);
        float xCoord = std::stof(x);
        float yCoord = std::stof(y);

        graph.push_back(new GraphNode(id, xCoord, yCoord));
    }
 
    // Read in the edges
    std::ifstream edgesFileStream(edgesFile);
    // std::string line;

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

// Read in a CSV grid representation of the map
// Each line is the same length and has a '.' for an empty space
// and a "#" for a wall
//
// Return a vector of vectors of chars
std::vector<std::vector<std::string>> buildMap(std::string mapFile){

    std::vector<std::vector<std::string>> map;
    std::ifstream mapFileStream(mapFile);
    std::string line;
    while (std::getline(mapFileStream, line))
    {
        std::vector<std::string> row;

        // add each string separated by a comma to the row

        std::stringstream lineStream(line);
        std::string cell;
        while (std::getline(lineStream, cell, ','))
        {
            row.push_back(cell);
        }
        
        map.push_back(row);
    }
    return map;
}

std::vector<GraphNode*> buildGraphFromMap(std::vector<std::vector<std::string>> map, float scale){
    std::vector<GraphNode*> graph;
    std::set<int> roomNumbers;
    for(int i = 0; i < map.size(); i++){
        for(int j = 0; j < map[i].size(); j++){
            if(map[i][j] == "d"){
                graph.push_back(new GraphNode(i*map[i].size() + j, j, i));
            }else if( map[i][j] == "s"|| map[i][j] == "g"){
                graph.push_back(new GraphNode(i*map[i].size() + j, j, i));
            }else if(map[i][j] != "#"){
                // add the number to the set of roomNumbers
                roomNumbers.insert(std::stoi(map[i][j]));
            }
        }
    }

    //print the room numbers
    for(auto it = roomNumbers.begin(); it != roomNumbers.end(); ++it){
        std::cout << *it << " ";
    }

    std::vector<std::vector<GraphNode*>> rooms;

    // initialize rooms with the size of the roomNumbers
    for(auto it = roomNumbers.begin(); it != roomNumbers.end(); ++it){
        std::vector<GraphNode*> room;
        rooms.push_back(room);
    }

    // For each node, if it is adjacent to a number on the map, add it to the room
    for(auto graphNode : graph){

        // get the x and y coordinates of the node
        int x = graphNode->x;
        int y = graphNode->y;



    }

    // print number of nodes in graph
    std::cout << "Number of nodes in graph: " << graph.size() << std::endl;

    // print size of rooms
    std::cout << "Number of rooms: " << rooms.size() << std::endl;

    // Print the node ids of each room
    for(auto room : rooms){
        std::cout << "Room: ";
        for(auto node : room){
            std::cout << node->id << " ";
        }
        std::cout << std::endl;
    }

    return graph;
}


#endif
