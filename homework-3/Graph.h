#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

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
// and a '#' for a wall
//
// Return a vector of vectors of chars
std::vector<std::vector<char>> buildMap(std::string mapFile){
    std::vector<std::vector<char>> map;
    std::ifstream mapFileStream(mapFile);
    std::string line;
    while (std::getline(mapFileStream, line))
    {
        std::vector<char> row;
        for(char c : line){
            if (c != ' '){
                row.push_back(c);
            }
        }
        map.push_back(row);
    }
    return map;
}

std::vector<GraphNode*> buildGraphFromMap(std::vector<std::vector<char>> map, float scale){
    std::vector<GraphNode*> graph;
    for(int i = 0; i < map.size(); i++){
        for(int j = 0; j < map[i].size(); j++){
            if(map[i][j] == 'd'){
                graph.push_back(new GraphNode(i*map[i].size() + j, j, i));
            }else if( map[i][j] == 's'|| map[i][j] == 'g'){
                graph.push_back(new GraphNode(i*map[i].size() + j, j, i));
            }
        }
    }

    // std::vector<std::vector<GraphNode*>> rooms;

    // // For each node, if it is adjacent to a number on the map, add it to the room
    // for(auto graphNode : graph){
    //     std::vector<GraphNode*> room;
    //     if(graphNode->x > 0 && map[graphNode->y][graphNode->x - 1] == '#'){
    //         room.push_back(graph[graphNode->y * map[0].size() + graphNode->x - 1]);
    //     }
    //     if(graphNode->x < map[0].size() - 1 && map[graphNode->y][graphNode->x + 1] == '#'){
    //         room.push_back(graph[graphNode->y * map[0].size() + graphNode->x + 1]);
    //     }
    //     if(graphNode->y > 0 && map[graphNode->y - 1][graphNode->x] == '#'){
    //         room.push_back(graph[(graphNode->y - 1) * map[0].size() + graphNode->x]);
    //     }
    //     if(graphNode->y < map.size() - 1 && map[graphNode->y + 1][graphNode->x] == '#'){
    //         room.push_back(graph[(graphNode->y + 1) * map[0].size() + graphNode->x]);
    //     }
    //     if(room.size() > 0){
    //         rooms.push_back(room);
    //     }
    // }

    // // For each room, add an edge between each node in the room
    // for(auto room : rooms){
    //     for(int i = 0; i < room.size(); i++){
    //         for(int j = i + 1; j < room.size(); j++){
    //             room[i]->addNeighbor(room[j], 1);
    //             room[j]->addNeighbor(room[i], 1);
    //         }
    //     }
    // }

    return graph;
}


#endif
