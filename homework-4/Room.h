#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include "GraphNode.h"
#include <iostream>
#include <list>

typedef std::list<std::pair<sf::Vector2f, float>> Obstacles;

struct room{
    int roomID;
    int x1; // the x coordinate of the top left corner of the room
    int y1; // the y coordinate of the top left corner of the room

    int x2; // the x coordinate of the bottom right corner of the room
    int y2; // the y coordinate of the bottom right corner of the room

    std::vector<GraphNode*> doors;
    Obstacles obstacles;
    // GraphNode* center;  

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

};

room getRoomFromCoordinates(std::vector<room> rooms, int x, int y){
    for(auto room : rooms){

        if (room.contains(x, y)){
            return room;
        }

    }
    std::cout << "ERROR: Could not find room from coordinates" << std::endl;
    return rooms[0];
}

bool compareByRoomID(room a, room b){
    return a.roomID < b.roomID;
}

#endif