#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include "GraphNode.h"
#include <iostream>
#include <list>
#include <unordered_map>

#include <cmath>

typedef std::list<std::pair<sf::Vector2f, float>> Obstacles;
// typedef std::pair<float, float> coordinate;

class room{

    public:

        int roomID;
        int x1; // the x coordinate of the top left corner of the room
        int y1; // the y coordinate of the top left corner of the room

        int x2; // the x coordinate of the bottom right corner of the room
        int y2; // the y coordinate of the bottom right corner of the room

        float jump_distance;

        std::vector<GraphNode*> doors;
        Obstacles obstacles;
        std::vector<GraphNode*> interiorGraph;

        room(int roomID, int x1, int y1, int x2, int y2){
            this->roomID = roomID;
            this->x1 = x1;
            this->y1 = y1;
            this->x2 = x2;
            this->y2 = y2;
            this->jump_distance = 20;
            doors = std::vector<GraphNode*>();
        }

        bool contains(int x, int y){
            return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
        }

        bool contains_with_padding(int x, int y, int padding){ // padding makes the room smaller than it actually is
            return (x >= x1 + padding && x <= x2 - padding && y >= y1 + padding && y <= y2 - padding);
        }

        sf::Vector2f getWall(int x, int y, int padding){ // padding makes it easier to get the wall

            if (x <= x1 + padding){
                return sf::Vector2f(1, 0);
            } else if ( x >= x2 - padding){
                return sf::Vector2f(-1, 0);
            } else if (y <= y1 + padding){
                return sf::Vector2f(0, 1);
            } else if (y >= y2 - padding){
                return sf::Vector2f(0, -1);
            } else {
                return sf::Vector2f(0, 0);
            }
        }

        sf::Vector2f getCenter(){
            return sf::Vector2f((x1 + x2) / 2, (y1 + y2) / 2);
        }


        int pair_to_int(std::pair<int, int> pair){
            return pair.first * 100 + pair.second;
        }
        std::pair<int,int> int_to_pair(int num){
            return std::pair<int,int>(num / 100, num % 100);
        }

        void addNodeToInteriorGraph(GraphNode* node){

            for(GraphNode* node2 : interiorGraph){
                float diff = node->diff(node2);
                if(diff < jump_distance + 5){

                    node->addNeighbor(node2, diff);
                    node2->addNeighbor(node, diff);
                }
            }

            for(GraphNode* node2 : doors){
                float diff = node->diff(node2);
                if(diff < jump_distance + 5){

                    node->addNeighbor(node2, diff);
                    node2->addNeighbor(node, diff);
                }
            }
            
            // interiorGraph.push_back(node);
        }

        // void deleteNodeFromInteriorGraph(GraphNode* node){
                
        //     for (auto neighbor : node->neighbors){
        //         neighbor.second->removeNeighbor(node);
        //     }

        //     interiorGraph.erase(std::remove(interiorGraph.begin(), interiorGraph.end(), node), interiorGraph.end());
        // }

        GraphNode* removeNodeFromGraph(GraphNode* node, std::vector<GraphNode*>& graph){
            for (int i = 0; i < graph.size(); i++){
                if (graph[i] == node){
                    graph.erase(graph.begin() + i);
                    return node;
                }
            }
            return nullptr;
        }


        std::vector<GraphNode*> buildGraphFromRoom(float padding){
            
            // for(auto obstacle : obstacles){
            //     std::cout << obstacle.first.x << " " << obstacle.first.y << " " << obstacle.second << std::endl;
            // }

            for(auto obstacle : obstacles){
                if(obstacle.second*2 < jump_distance){
                    this->jump_distance = obstacle.second*2;
                }
            }

            int rows = round ((y2-y1 - 1*padding) / jump_distance);
            int cols = round((x2-x1 - 1*padding) / jump_distance);

            std::unordered_map <int, GraphNode*> coordinateGraph;

            for (int i = 0; i < rows; i++){
                for (int j = 0; j < cols; j++){
                    int x = padding + x1 + j * jump_distance;
                    int y = padding + y1 + i * jump_distance;
                    int coordinate = pair_to_int(std::pair<int,int>(x,y));
                    coordinateGraph[coordinate] = new GraphNode(coordinate ,x, y);
                }
            }

            // for each obstacle, remove any nodes that are within the obstacle radius + padding
            for (std::pair<sf::Vector2f, float> obstacle : obstacles){
                int x = obstacle.first.x;
                int y = obstacle.first.y;
                int radius = obstacle.second;

                for (int i = 0; i < rows; i++){
                    for (int j = 0; j < cols; j++){
                        int x_coord = padding + x1 + j * jump_distance;
                        int y_coord = padding + y1 + i * jump_distance;
                        int coordinate = pair_to_int(std::pair<int,int>(x_coord,y_coord));
                        if (sqrt(pow(x_coord - x, 2) + pow(y_coord - y, 2)) <= radius + padding){
                            coordinateGraph[coordinate] = nullptr;
                        }
                    }
                }
            }

            int diagonal_jump_distance =  round( sqrt(2*jump_distance*jump_distance));

            for (int i = 0; i < rows; i++){
                for (int j = 0; j < cols; j++){
                    int x = padding + x1 + j * jump_distance;
                    int y = padding + y1 + i * jump_distance;
                    int coordinate = pair_to_int(std::pair<int,int>(x,y));
                    GraphNode* node = coordinateGraph[coordinate];

                    // skip if the node is null
                    if (node == nullptr){
                        continue;
                        // std::cout << "skipping obstacle node" << std::endl;
                    }

                    // connect to horizontal and vertical neighbors
                    if (i > 0){
                        int up = pair_to_int(std::pair<int,int>(x,y-jump_distance));

                        if (coordinateGraph[up] != nullptr){
                            node->addNeighbor(coordinateGraph[up], jump_distance);
                        }

                    }
                    if (i < rows - 1){
                        int down = pair_to_int(std::pair<int,int>(x,y+jump_distance));

                        if (coordinateGraph[down] != nullptr){
                            node->addNeighbor(coordinateGraph[down], jump_distance);
                        }

                    }
                    if (j > 0){
                        int left = pair_to_int(std::pair<int,int>(x-jump_distance,y));

                        if (coordinateGraph[left] != nullptr){
                            node->addNeighbor(coordinateGraph[left], jump_distance);
                        }

                    }
                    if (j < cols - 1){
                        int right = pair_to_int(std::pair<int,int>(x+jump_distance,y));

                        if (coordinateGraph[right] != nullptr){
                            node->addNeighbor(coordinateGraph[right], jump_distance);
                        }

                    }

                    // connect to diagonal neighbors
                    if (i > 0 && j > 0){
                        int up_left = pair_to_int(std::pair<int,int>(x-jump_distance,y-jump_distance));

                        if (coordinateGraph[up_left] != nullptr){
                            node->addNeighbor(coordinateGraph[up_left], diagonal_jump_distance);
                        }

                    }
                    if (i > 0 && j < cols - 1){
                        int up_right = pair_to_int(std::pair<int,int>(x+jump_distance,y-jump_distance));

                        if (coordinateGraph[up_right] != nullptr){
                            node->addNeighbor(coordinateGraph[up_right], diagonal_jump_distance);
                        }

                    }
                    if (i < rows - 1 && j > 0){
                        int down_left = pair_to_int(std::pair<int,int>(x-jump_distance,y+jump_distance));

                        if (coordinateGraph[down_left] != nullptr){
                            node->addNeighbor(coordinateGraph[down_left], diagonal_jump_distance);
                        }

                    }
                    if (i < rows - 1 && j < cols - 1){
                        int down_right = pair_to_int(std::pair<int,int>(x+jump_distance,y+jump_distance));

                        if (coordinateGraph[down_right] != nullptr){
                            node->addNeighbor(coordinateGraph[down_right], diagonal_jump_distance);
                        }

                    }
                }
            }

            std::vector<GraphNode*> graph;

            // add nodes to graph and ignore null ptrs
            for (auto it = coordinateGraph.begin(); it != coordinateGraph.end(); ++it){

                GraphNode* node = it->second;

                if (node != nullptr){

                    // if node is near wall, check if door is in range and add edge
                    int adj_to_wall_padding = (jump_distance + 1.5*padding) ;

                    if (node->x < x1 + adj_to_wall_padding  || node->x > x2 - adj_to_wall_padding || node->y < y1 + adj_to_wall_padding || node->y > y2 - adj_to_wall_padding ){
                        for (auto door : doors){
                            float diff = door->diff(node);

                            if (diff <= adj_to_wall_padding){
                                node->addNeighbor(door, diff);
                                door->addNeighbor(node, diff);

                            }
                        }
                    }

                    graph.push_back(it->second);
                }
            }

            interiorGraph = graph;

            return graph;
        }
};


room * getRoomFromCoordinates(std::vector<room * > rooms, int x, int y){

    for (auto room : rooms){
        if (room->contains(x, y)){
            return room;
        }
    }

    std::cout << "ERROR: Could not find room from coordinates" << std::endl;
    room * errorRoom = new room(-1,-1,-1,-1,-1);
    // errorRoom.roomID = -1;

    return errorRoom;
}

bool compareByRoomID(room * a, room * b){
    return a->roomID < b->roomID;
}


#endif