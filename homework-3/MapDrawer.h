#ifndef MAPDRAWER_H
#define MAPDRAWER_H

#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>
#include "GraphNode.h"

// A class to render a map 

class MapDrawer {
public:

    std::vector<std::vector<std::string>> map;
    float scale;


    std::vector<sf::RectangleShape> rects;
    std::vector<sf::CircleShape> circles;
    std::vector<sf::RectangleShape> lines;

    MapDrawer(std::vector<std::vector<std::string>> map, std::vector<GraphNode*> Nodes ,float scale){
        this->map = map;
        this->scale = scale;

        for(int i = 0; i < map.size(); i++){
            for(int j = 0; j < map[i].size(); j++){
                if(map[i][j] == "#"){
                    sf::RectangleShape rect(sf::Vector2f(scale, scale));
                    rect.setPosition(j*scale, i*scale);
                    rect.setFillColor(sf::Color::Black);
                    rects.push_back(rect);
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
                // sf::RectangleShape line = lineBetweenPoints(sf::Vector2f(node->x, node->y), sf::Vector2f(neighbor.second->x, neighbor.second->y), 1);
                // lines.push_back(line);
        }

    }

    // ~MapDrawer(){
    //     rects.clear();
    //     circles.clear();
    //     lines.clear();
    // }

    // sf::RectangleShape lineBetweenPoints(sf::Vector2f start, sf::Vector2f end, float thickness){
    //     sf::RectangleShape line(sf::Vector2f(end.x - start.x, end.y - start.y));
    //     line.setPosition(start);
    //     line.setFillColor(sf::Color::Black);
    //     line.setOutlineColor(sf::Color::Black);
    //     line.setOutlineThickness(thickness);
    //     return line;
    // }

    void drawMap(sf::RenderWindow& window){
        for(int i = 0; i < rects.size(); i++){
            window.draw(rects[i]);
        } 
    }

    void drawOverlay(sf::RenderWindow& window){
        // for each node in Nodes, draw a circle at the node's position

        for(int i = 0; i < circles.size(); i++){
            window.draw(circles[i]);
        }
    }

};

#endif