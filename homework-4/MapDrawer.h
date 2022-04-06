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

    sf::Font font;

    std::vector<sf::RectangleShape> walls;
    std::vector<sf::RectangleShape> floors;
    std::vector<sf::CircleShape> circles;
    std::vector<sf::RectangleShape> lines;

    std::vector<sf::Text> door_numbers;
    std::vector<sf::Text> room_numbers;

    MapDrawer(std::vector<std::vector<std::string>> map, std::vector<GraphNode*> Nodes ,std::vector<room> rooms ,float scale){
        this->map = map;
        this->scale = scale;

        if (!font.loadFromFile("fonts/arial.ttf")){
            printf("Could not load font\n");
        };

        for(int i = 0; i < map.size(); i++){
            for(int j = 0; j < map[i].size(); j++){
                if(map[i][j] == "#"){

                    // wall to the right
                    bool wall_right = false;
                    // wall below
                    bool wall_below = false;

                    bool wall_below_and_left = false;
                    bool wall_below_and_right = false;

                    // if there is a wall to the right,
                    if(j+1 < map[i].size() && map[i][j+1] == "#"){
                        wall_right = true;
                    }

                    // if there is a wall below,
                    if(i+1 < map.size() && map[i+1][j] == "#"){
                        wall_below = true;
                    }

                    // if there is a wall below and to the left,
                    if(i+1 < map.size() && j-1 >= 0 && map[i+1][j-1] == "#"){
                        wall_below_and_left = true;
                    }

                    // if there is a wall below and to the right,
                    if(i+1 < map.size() && j+1 < map[i].size() && map[i+1][j+1] == "#"){
                        wall_below_and_right = true;
                    }

                    if (wall_right){
                        sf::RectangleShape rect(sf::Vector2f(scale,scale/2));
                        rect.setPosition(j*scale - scale/2, i*scale - scale/2);
                        rect.setFillColor(sf::Color::Black);
                        walls.push_back(rect);

                    } if (wall_below){
                        sf::RectangleShape rect(sf::Vector2f(scale/2,scale));
                        rect.setPosition(j*scale - scale/2, i*scale - scale/2);
                        rect.setFillColor(sf::Color::Black);
                        walls.push_back(rect);

                    } if (!wall_right && !wall_below){

                        if (wall_below_and_left){
                            sf::RectangleShape rect(sf::Vector2f(scale/2,scale*1.70));
                            rect.setPosition(j*scale - scale/2, i*scale - scale/2);

                            rect.rotate(45);
                            
                            rect.setFillColor(sf::Color::Black);
                            walls.push_back(rect);

                        }

                        if (wall_below_and_right){
                            sf::RectangleShape rect(sf::Vector2f(scale/2,scale*1.70));
                            rect.setPosition(j*scale - scale/2, i*scale - scale/2);

                            rect.rotate(-45);
                            
                            rect.setFillColor(sf::Color::Black);
                            walls.push_back(rect);

                        }
                    }
                }

            }
        }

        for(auto node : Nodes){
            sf::CircleShape circle(scale/2);
            circle.setPosition(node->x - scale/2, node->y - scale/2);
            circle.setFillColor(sf::Color::White);
            circles.push_back(circle);

            // for each neighbor of the node, draw a line between the node and the neighbor
            for(auto neighbor : node->neighbors){
                // sf::RectangleShape line = lineBetweenPoints(sf::Vector2f(node->x + scale/2, node->y + scale/2), sf::Vector2f(neighbor.second->x + scale/2, neighbor.second->y + scale/2), 1);
                sf::RectangleShape line = lineBetweenPoints(sf::Vector2f(node->x , node->y ), sf::Vector2f(neighbor.second->x, neighbor.second->y ), 1);
                lines.push_back(line);
            }

            // for each node, draw a text with the node's ID
            sf::Text text;

            text.setFont(font);
            text.setCharacterSize(18);
            text.setColor(sf::Color::Red);
            text.setString(std::to_string(node->id));
            text.setPosition(node->x + scale/2 - text.getGlobalBounds().width/2, node->y + scale/2 - text.getGlobalBounds().height/2);
            text.setStyle(sf::Text::Bold);
            door_numbers.push_back(text);

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
            floors.push_back(rect);

            // for each room, draw a text with the room's ID
            sf::Text text;

            text.setFont(font);
            text.setCharacterSize(18);
            text.setColor(sf::Color::Black);
            text.setString(std::to_string(room.roomID));
            text.setPosition(x1 + (x2-x1)/2 - text.getGlobalBounds().width/2, y1 + (y2-y1)/2 - text.getGlobalBounds().height/2);
            text.setStyle(sf::Text::Bold);
            room_numbers.push_back(text);

        }

    }

    ~MapDrawer(){
        walls.clear();
        circles.clear();
        lines.clear();
        floors.clear();
        door_numbers.clear();
        room_numbers.clear();
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

        for(int i = 0; i < door_numbers.size(); i++){
            window.draw(door_numbers[i]);
        }

        for (int i = 0; i < room_numbers.size(); i++){
            window.draw(room_numbers[i]);
        }
    }

    void colorRooms(sf::RenderWindow& window){
        for(int i = 0; i < map.size(); i++){
            for(int j = 0; j < map[i].size(); j++){
                if(map[i][j] == "R"){
                    sf::RectangleShape rect(sf::Vector2f(scale, scale));
                    // rect.setPosition(j*scale, i*scale);
                    rect.setPosition(j*scale + scale/2, i*scale + scale/2);
                    rect.setFillColor(sf::Color::Blue);
                    walls.push_back(rect);
                }
            }
        }
    }

    

};

#endif