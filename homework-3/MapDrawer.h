#ifndef MAPDRAWER_H
#define MAPDRAWER_H

#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>
#include "GraphNode.h"
#include "from-homework-2/HelperFunctions.h"
#include "Graph.h"

// A class to render a map 

class MapDrawer {
public:

    std::vector<std::vector<std::string>> map;
    float scale;


    std::vector<sf::RectangleShape> walls;
    std::vector<sf::RectangleShape> floors;
    std::vector<sf::CircleShape> circles;
    std::vector<sf::RectangleShape> lines;
    // std::vector<sf::Text> texts;

    MapDrawer(std::vector<std::vector<std::string>> map, std::vector<GraphNode*> Nodes ,std::vector<room> rooms ,float scale){
        this->map = map;
        this->scale = scale;

        for(int i = 0; i < map.size(); i++){
            for(int j = 0; j < map[i].size(); j++){
                if(map[i][j] == "#"){
                    sf::RectangleShape rect(sf::Vector2f(scale, scale));
                    rect.setPosition(j*scale, i*scale);
                    rect.setFillColor(sf::Color::Black);
                    walls.push_back(rect);
                }

            }
        }


        for(auto node : Nodes){
            sf::CircleShape circle(scale/2);
            circle.setPosition(node->x, node->y);
            circle.setFillColor(sf::Color::Red);
            circles.push_back(circle);

            // for each neighbor of the node, draw a line between the node and the neighbor
            for(auto neighbor : node->neighbors){
                sf::RectangleShape line = lineBetweenPoints(sf::Vector2f(node->x + scale/2, node->y + scale/2), sf::Vector2f(neighbor.second->x + scale/2, neighbor.second->y + scale/2), 1);
                lines.push_back(line);
            }

        }

        for(auto room : rooms){

            
            int x1; // the x coordinate of the top left corner of the room
            int y1; // the y coordinate of the top left corner of the room

            int x2; // the x coordinate of the bottom right corner of the room
            int y2; // the y coordinate of the bottom right corner of the room

            x1 = room.x1;
            y1 = room.y1;

            x2 = room.x2;
            y2 = room.y2;

            sf::RectangleShape rect(sf::Vector2f(x2-x1, y2-y1));
            rect.setPosition(x1, y1);

            // set color to room number
            int roomNum = room.roomID;

            sf::Color color = sf::Color(roomNum*20, roomNum*35, roomNum*50 % 255, 124);

            rect.setFillColor(color);
            // rect.setFillColor();
            floors.push_back(rect);


        }

    }

    ~MapDrawer(){
        walls.clear();
        circles.clear();
        lines.clear();
        floors.clear();
    }

    sf::RectangleShape lineBetweenPoints(sf::Vector2f start, sf::Vector2f end, float thickness){
        
        auto diff = end - start;

        float distance = magnitude( diff );

        sf::RectangleShape line(sf::Vector2f(distance, thickness));


        line.setPosition(start);
        line.setFillColor(sf::Color::Blue);

        line.setRotation(atan2(end.y - start.y, end.x - start.x) * 180 / 3.14159265);

        return line;
    }

    void drawMap(sf::RenderWindow& window){
        for(int i = 0; i < walls.size(); i++){
            window.draw(walls[i]);
        } 
    }

    void drawFloors(sf::RenderWindow& window){
        for(int i = 0; i < floors.size(); i++){
            window.draw(floors[i]);
        } 
    }

    void drawOverlay(sf::RenderWindow& window){
        // for each node in Nodes, draw a circle at the node's position

        for(int i = 0; i < circles.size(); i++){
            window.draw(circles[i]);
           
        }

        for(int i = 0; i < lines.size(); i++){
            window.draw(lines[i]);
        }
    }

    void colorRooms(sf::RenderWindow& window){
        for(int i = 0; i < map.size(); i++){
            for(int j = 0; j < map[i].size(); j++){
                if(map[i][j] == "R"){
                    sf::RectangleShape rect(sf::Vector2f(scale, scale));
                    rect.setPosition(j*scale, i*scale);
                    rect.setFillColor(sf::Color::Blue);
                    walls.push_back(rect);
                }
            }
        }
    }

};

#endif