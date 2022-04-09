#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
#include <math.h>
#include <algorithm>

#include "GraphNode.h"
#include "Room.h"

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

GraphNode* getGraphNodeFromCoordinates(std::vector<GraphNode*> graph, int x, int y){
    for(auto node : graph){
        if(node->x == x && node->y == y){
            return node;
        }
    }
    std::cout << "ERROR: Could not find node from coordinates" << std::endl;
    return graph[0];
}

struct buildGraphFromMapReturn{
    std::vector<GraphNode*> graph;
    std::vector<room> rooms;
};


buildGraphFromMapReturn buildGraphFromMap(std::vector<std::vector<std::string>> map, float scale){
    std::vector<GraphNode*> graph;

    std::vector<room> rooms;

    buildGraphFromMapReturn returnStruct;


    // Get each of the rooms in the map
    for(int i = 0; i < map.size(); i++){
        for(int j = 0; j < map[i].size(); j++){

            if(map[i][j] == "d"){
                graph.push_back(new GraphNode(i*map[i].size() + j, j, i));
            }

            // if the string starts with an 'r'
            else if (map[i][j].at(0) == 'r'){

                // get the room number (the number after the 'r')
                int roomNum = std::stoi(map[i][j].substr(1));

                room newRoom;

                newRoom.roomID = roomNum;

                // loop through the map and find the top left corner of the room
                int x1 = j -1;
                int y1 = i -1;

                while(map[i][x1] != "#" && map[i][x1] != "d"){
                    x1--;
                }
                // x1++;

                while(map[y1][j] != "#" && map[y1][j] != "d" ){
                    y1--;
                }
                // y1 ++;

                newRoom.x1 = x1 * scale;
                newRoom.y1 = y1 * scale;

                // loop through the map and find the bottom right corner of the room
                int x2 = j + 1;
                int y2 = i + 1;

                while(map[i][x2] != "#" && map[i][x2] != "d"){
                    x2++;
                }


                while(map[y2][j] != "#" &&  map[y2][j] != "d"){
                    y2++;
                }


                newRoom.x2 = x2 * scale;
                newRoom.y2 = y2 * scale;

                newRoom.doors = std::vector<GraphNode*>();
                rooms.push_back(newRoom);

            }
        }
    }

    // sort the rooms by roomID
    std::sort(rooms.begin(), rooms.end(), compareByRoomID);

    // get each of the obstacles in the map
    for(int i = 0; i < map.size(); i++){
        for(int j = 0; j < map[i].size(); j++){

            if(map[i][j] == "O"){

                float random_obstacle_size = 0.2* scale * (rand() % 3) + 4;

                rooms[getRoomFromCoordinates(rooms, j*scale, i*scale).roomID].obstacles.push_back({sf::Vector2f(j*scale,i*scale), random_obstacle_size});

            }
        }
    }


    // For each node, if it is adjacent to a number on the map, add it to the room
    std::string tile = "";
    for(auto graphNode : graph){

        // get the x and y coordinates of the node
        int x = graphNode->x;
        int y = graphNode->y;

        // check the map tile above the node
        tile = map[y-1][x];
        if(y > 0 && tile != "#" &&  tile != "s" && tile != "g"){

            if (tile != "d"){
                // add the node to the room number room
                rooms[std::stoi(tile)].doors.push_back(graphNode);
            } else{
                // add an edge between the node and the node above it
                // is necessary because adjacent doors may be in different rooms
                graphNode->addNeighbor(getGraphNodeFromCoordinates(graph, x, y-1), 1.0f * scale);
            }

        }

        // check the map tile below the node
        tile = map[y+1][x];
        if(y < map.size()-1 && tile != "#" &&  tile != "s" && tile != "g"){
            // add the node to the room number room

            if (tile != "d"){
                // add the node to the room number room
                rooms[std::stoi(tile)].doors.push_back(graphNode);
            } else{
                // add an edge between the node and the node below it
                // is necessary because adjacent doors may be in different rooms
                graphNode->addNeighbor(getGraphNodeFromCoordinates(graph, x, y+1), 1.0f * scale);
            }

        }

        // check the map tile to the left of the node
        tile = map[y][x-1];
        if(x > 0 && tile != "#" &&  tile != "s" && tile != "g"){
            // add the node to the room number room

            if (tile != "d"){
                // add the node to the room number room
                rooms[std::stoi(tile)].doors.push_back(graphNode);
            }else{
                // add an edge between the node and the node to the left of it
                // is necessary because adjacent doors may be in different rooms
                graphNode->addNeighbor(getGraphNodeFromCoordinates(graph, x-1, y), 1.0f * scale);
                
            }

        }

        // check the map tile to the right of the node
        tile = map[y][x+1];
        if(x < map[y].size()-1 && tile != "#" &&  tile != "s" && tile != "g"){
            // add the node to the room number room

            if (tile != "d"){
                // add the node to the room number room
                rooms[std::stoi(tile)].doors.push_back(graphNode);
            } else{
                // add an edge between the node and the node to the right of it
                // is necessary because adjacent doors may be in different rooms
                graphNode->addNeighbor(getGraphNodeFromCoordinates(graph, x+1, y), 1.0f * scale);
                
            }

        }
    }

    float wallOffset = 5.0f;

    // for each node, multiply its position by the scale factor
    for(auto node : graph){
        
        node->x *= scale;
        node->y *= scale;
        
        node->x -= wallOffset;
        node->y -= wallOffset;

    }

    // for each node in each room, add an edge to every other node in the room with a cost equal to the euclidean distance between the two nodes
    for(auto room : rooms){
        for(auto node : room.doors){
            for(auto otherNode : room.doors){
                if(node != otherNode){
                    node->addNeighbor(otherNode, sqrt(pow(node->x - otherNode->x, 2) + pow(node->y - otherNode->y, 2)));
                }
            }
        }
    }


    returnStruct.graph = graph;
    returnStruct.rooms = rooms;

    return returnStruct;
}


std::vector<GraphNode*> addNodeToGraph(std::vector<GraphNode*> graph, std::vector<room> rooms, GraphNode* node){

    // get the room that the node is in
    room room = getRoomFromCoordinates(rooms, node->x, node->y);
    
    // add an edge to and from every other node in the room
    for(auto otherNode : room.doors){
        if(otherNode != node){

            node->addNeighbor(otherNode, sqrt(pow(node->x - otherNode->x, 2) + pow(node->y - otherNode->y, 2)));
            otherNode->addNeighbor(node, sqrt(pow(otherNode->x - node->x, 2) + pow(otherNode->y - node->y, 2)));
        }
    }

    return graph;
}

std::vector<GraphNode*> removeNodeFromGraph(std::vector<GraphNode*> graph, GraphNode* node){

    // remove each of the node's neighbor's edges to the node
    for(auto neighbor : node->neighbors){
        neighbor.second->removeNeighbor(node);
    }
    // remove the node's edges to its neighbors
    node->neighbors.clear();

    node->visited = false;
    node->parent = NULL;
    node->g = INF;
    node->h = INF;

    return graph;
}

std::vector<GraphNode*> cleanGraph(std::vector<GraphNode*> graph){
    for(auto node : graph){
        node->visited = false;
        node->parent = NULL;
        node->g = INF;
        node->h = INF;
    }
    return graph;
}


#endif
