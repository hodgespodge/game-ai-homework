#ifndef LOCALNAVFUNCTIONS_H
#define LOCALNAVFUNCTIONS_H

#include "Graph.h"
#include "SFML/Graphics.hpp"
#include <list>
#include "from-homework-2/HelperFunctions.h"
// #include <unordered_map>


typedef std::pair<int, int> coordinate;

// Obstacles is a list of pairs of sf::Vector2f and float (radius)
typedef std::list<std::pair<sf::Vector2f, float>> Obstacles;

std::vector<sf::Vector2f> coordinateBasedAStar(sf::Vector2f start, sf::Vector2f end, room& room, Obstacles obstacles, float jump_distance);

std::vector<sf::Vector2f> getPathThroughRoom(sf::Vector2f start, sf::Vector2f end, room& room, Obstacles obstacles){
    // std::cout << "getPathThroughRoom" << std::endl;

    float jump_distance = 10;

    // // set jumpDistance to the size of the smallest obstacle
    // for (auto it = obstacles.begin(); it != obstacles.end(); ++it){
    //     if (it->second < jump_distance){
    //         jump_distance = it->second;
    //     }
    // }

    // Perform A* search to find the path from start to end in the room with obstacles in it (obstacles) 
    // The path should be a vector of sf::Vector2f

    std::vector<sf::Vector2f> localPath = coordinateBasedAStar(start, end, room, obstacles, jump_distance);

    return localPath;
 

}

sf::Vector2f rotatedVector(sf::Vector2f vec, float angle){
    sf::Vector2f newVec = sf::Vector2f(0,0);
    newVec.x = vec.x * cos(angle) - vec.y * sin(angle);
    newVec.y = vec.x * sin(angle) + vec.y * cos(angle);
    return newVec;
}

float distance(coordinate start, coordinate end){
    return sqrt(pow(start.first - end.first, 2) + pow(start.second - end.second, 2));
}

float heuristic_cost_estimate(coordinate start, coordinate end){
    return distance(start, end);
}


std::vector<sf::Vector2f> coordinateBasedAStar(sf::Vector2f start, sf::Vector2f end, room& room, Obstacles obstacles, float jump_distance){

    coordinate start_coordinate = {start.x, start.y};
    coordinate end_coordinate = {end.x, end.y};

    float obstacle_padding = 6;

    // std::cout << "coordinateBasedAStar" << std::endl;
    // std::cout << "navigation start: " << start_coordinate.first << ", " << start_coordinate.second << std::endl;
    // std::cout << "navigation end: " << end_coordinate.first << ", " << end_coordinate.second << std::endl;

    std::set<coordinate> openSet;

    std::map<coordinate, coordinate> cameFrom;
    std::map<coordinate, float> gScore;
    std::map<coordinate, float> fScore;

    openSet.insert({start.x, start.y});

    gScore[{start.x, start.y}] = 0;
    fScore[{start.x, start.y}] = heuristic_cost_estimate(start_coordinate, end_coordinate);

    while (openSet.size() > 0){

        coordinate current = *openSet.begin();

        for (auto it = openSet.begin(); it != openSet.end(); ++it){ // find the node with the lowest fScore
            if (fScore[*it] < fScore[current]){
                current = *it;
            }
        }

        if (distance(current, end_coordinate) < jump_distance + 1){ // if within range of target

            std::vector<sf::Vector2f> path = std::vector<sf::Vector2f>();

            path.push_back(sf::Vector2f(current.first, current.second));

            while (cameFrom.find(current) != cameFrom.end()){
                current = cameFrom[current];

                path.push_back(sf::Vector2f(current.first, current.second));

            }
            return path;
        }

        openSet.erase(current);
        

        // Calculate the neighboring nodes

        sf::Vector2f current_as_vector = sf::Vector2f(current.first, current.second);
        sf::Vector2f goalOffset = sf::Vector2f(end_coordinate.first - current.first, end_coordinate.second - current.second);
        sf::Vector2f goalDirection = unitVector( goalOffset);
        // rotate goalDirection by 30 degrees
        sf::Vector2f leftDirection = rotatedVector(goalDirection, 30);
        sf::Vector2f rightDirection = rotatedVector(goalDirection, -30);

        std::vector<coordinate> neighbors;

        sf::Vector2f straight = current_as_vector + (goalDirection * jump_distance);
        sf::Vector2f left = current_as_vector + (leftDirection * jump_distance);
        sf::Vector2f right = current_as_vector + (rightDirection * jump_distance);

        neighbors.push_back({straight.x, straight.y});
        neighbors.push_back({left.x, left.y});
        neighbors.push_back({right.x, right.y});

        for (auto it = neighbors.begin(); it != neighbors.end(); ++it){

            if(!room.contains_with_padding(it->first, it->second, obstacle_padding)){
                continue; // skip this neighbor
            }

            // check if the neighbor point is within range of any obstacles using distance
            for (auto obstacle = obstacles.begin(); obstacle != obstacles.end(); ++obstacle){
                if (distance(coordinate{it->first, it->second}, coordinate{obstacle->first.x, obstacle->first.y}) < obstacle->second - obstacle_padding){
                    continue; // skip this neighbor
                }
            }
            
            float tentative_gScore = gScore[current] + jump_distance;

            if(gScore.find(*it) == gScore.end()){

                gScore.insert({*it, 10000});
                fScore.insert({*it, 10000});
            }

            if (tentative_gScore < gScore[*it]){
                cameFrom[*it] = current;
                gScore[*it] = tentative_gScore;
                fScore[*it] = gScore[*it] + heuristic_cost_estimate(*it, end_coordinate);
                if (openSet.find(*it) == openSet.end()){

                    openSet.insert(*it);
                }
            }

        }

    }

    std::cout << "no path found between start " << start.x << "," << start.y << " and end " << end.x << "," << end.y << std::endl;

    return std::vector<sf::Vector2f>(); // return empty vector if no path found

}  


#endif