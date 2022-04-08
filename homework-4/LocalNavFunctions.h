#ifndef LOCALNAVFUNCTIONS_H
#define LOCALNAVFUNCTIONS_H

#include "Graph.h"
#include "SFML/Graphics.hpp"
#include <list>
#include "from-homework-2/HelperFunctions.h"


typedef std::list<std::pair<sf::Vector2f, float>> Obstacles;
// Obstacles is a list of pairs of sf::Vector2f and float (distance)

std::vector<sf::Vector2f> getPathThroughRoom(sf::Vector2f start, sf::Vector2f end, room& room, Obstacles obstacles){
    std::cout << "getPathThroughRoom" << std::endl;

    std::vector<sf::Vector2f> path = std::vector<sf::Vector2f>();
    path.push_back(start);

    if (obstacles.size() == 0){
        path.push_back(end);
        return path;
    }

    // sort obstacles by distance from start
    Obstacles sortedObstacles = Obstacles();
    for (auto obstacle : obstacles){
        float distance = sqrt(pow(obstacle.first.x - start.x, 2) + pow(obstacle.first.y - start.y, 2));
        sortedObstacles.push_back(std::make_pair(obstacle.first, distance));
    }

    sortedObstacles.sort([](std::pair<sf::Vector2f, float> a, std::pair<sf::Vector2f, float> b){
        return a.second < b.second;
    });

    // for each obstacle, find the closest point on the path to the obstacle
    for (auto obstacle : sortedObstacles){
        sf::Vector2f closestPoint = sf::Vector2f(0,0);
        float closestDistance = std::numeric_limits<float>::max();
        for (auto point : path){
            float distance = sqrt(pow(point.x - obstacle.first.x, 2) + pow(point.y - obstacle.first.y, 2));
            if (distance < closestDistance){
                closestPoint = point;
                closestDistance = distance;
            }
        }

        // if closest distance is less than the obstacle's radius, move the closest point out of the obstacle

        if (closestDistance < obstacle.second){
            sf::Vector2f direction = unitVector(closestPoint - obstacle.first);
            closestPoint = obstacle.first + direction * (obstacle.second - closestDistance);
        }

        path.push_back(closestPoint);


        
    }

    




}

#endif