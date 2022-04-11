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

    // float jump_distance = 10;
    float jump_distance = 12;

    // // set the jump_distance to the size of the smallest obstacle radius
    // for(auto obstacle : obstacles){
    //     if(obstacle.second < jump_distance){
    //         jump_distance = obstacle.second;
    //     }
    // }

    // std::cout << "jump_distance: " << jump_distance << std::endl;

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

float distance(float x1, float y1, float x2, float y2){
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

float heuristic_cost_estimate(coordinate start, coordinate end){
    return distance(start, end);
}



std::vector<sf::Vector2f> coordinateBasedAStar(sf::Vector2f start, sf::Vector2f end, room& room, Obstacles obstacles, float jump_distance){

    coordinate start_coordinate = {start.x, start.y};
    coordinate end_coordinate = {end.x, end.y};

    sf::Vector2f start_direction = end - start;
    start_direction = unitVector(start_direction);

    // std::cout << "approach direction: " <<  start_direction.x << ", " << start_direction.y << std::endl;

    


    // float obstacle_padding = 10;
    float obstacle_padding = jump_distance;

    std::set<std::pair< float ,coordinate>> openSet;
    std::set<coordinate> closedSet;

    std::map<coordinate, coordinate> cameFrom;
    std::map<coordinate, float> gScore;
    bool first_iteration = true;

    gScore[{start.x, start.y}] = 0;
    // fScore[{start.x, start.y}] = heuristic_cost_estimate(start_coordinate, end_coordinate);

    openSet.insert({heuristic_cost_estimate(start_coordinate, end_coordinate),{start.x, start.y}});

    while (openSet.size() > 0){

        coordinate current = openSet.begin()->second;
        openSet.erase(openSet.begin());

        closedSet.insert(current);

        if (distance(current, end_coordinate) < jump_distance*2){ // if within range of target

            std::vector<sf::Vector2f> path = std::vector<sf::Vector2f>();

            path.push_back(sf::Vector2f(current.first, current.second));

            while (cameFrom.find(current) != cameFrom.end()){
                current = cameFrom[current];

                path.push_back(sf::Vector2f(current.first, current.second));

            }

            return path;
        }

        // Calculate the neighboring nodes

        sf::Vector2f current_as_vector = sf::Vector2f(current.first, current.second);
        
        sf::Vector2f spriteMovementDirection;

        if (first_iteration){

            spriteMovementDirection = start_direction;

            first_iteration = false;
            
        }else{
            coordinate previousCoord = cameFrom.at(current);
            sf::Vector2f previousOffset = sf::Vector2f(previousCoord.first - current.first, previousCoord.second - current.second);
            spriteMovementDirection = unitVector(previousOffset);
        }

        // rotate spriteMovementDirection by 10 degrees
        sf::Vector2f leftDirection = rotatedVector(spriteMovementDirection, 10);
        sf::Vector2f leftleftDirection = rotatedVector(leftDirection, 10);
        sf::Vector2f rightDirection = rotatedVector(spriteMovementDirection, -10);
        sf::Vector2f rightrightDirection = rotatedVector(rightDirection, -10);

        std::vector<coordinate> neighbors;

        sf::Vector2f straight = current_as_vector + (spriteMovementDirection * jump_distance);
        sf::Vector2f left = current_as_vector + (leftDirection * jump_distance);
        sf::Vector2f right = current_as_vector + (rightDirection * jump_distance);

        sf::Vector2f leftleft = current_as_vector + (leftleftDirection * jump_distance);
        sf::Vector2f rightright = current_as_vector + (rightrightDirection * jump_distance);

        neighbors.push_back({straight.x, straight.y});
        neighbors.push_back({left.x, left.y});
        neighbors.push_back({right.x, right.y});
        neighbors.push_back({leftleft.x, leftleft.y});
        neighbors.push_back({rightright.x, rightright.y});

        for (auto it = neighbors.begin(); it != neighbors.end(); ++it){

            // std::cout << "Checking neighbor: " << it->first << ", " << it->second << std::endl;

            if(closedSet.find(*it) != closedSet.end()){
                // std::cout << "already checked... passing" << std::endl;
                continue;
            }

            if(!room.contains_with_padding(it->first, it->second, obstacle_padding)){
                // std::cout << "out of bounds... passing" << std::endl;
                continue; // skip this neighbor
            }

            bool obstacle_collision = false;

            // check if the neighbor point is within range of any obstacles using distance
            for (auto obstacle = obstacles.begin(); obstacle != obstacles.end(); ++obstacle){

                if (distance(it->first, it->second, obstacle->first.x, obstacle->first.y) - obstacle_padding  < obstacle->second ){
                    // std::cout << "obstacle collision... passing" << std::endl;
                    obstacle_collision = true;
                    continue; // skip this neighbor
                }
            }

            if (obstacle_collision){
                // std::cout << "obstacle collision... passing" << std::endl;
                continue; // skip this neighbor
            }
            
            float tentative_gScore = gScore[current] + jump_distance;

            if(gScore.find(*it) == gScore.end()){

                gScore.insert({*it, 10000});
            }

            // std::cout << "neighbor being added to openSet: " << it->first << ", " << it->second << std::endl;

            if (tentative_gScore < gScore[*it]){
                cameFrom[*it] = current;
                gScore[*it] = tentative_gScore;

                openSet.insert({tentative_gScore + heuristic_cost_estimate(*it, end_coordinate), *it});

            }

        }

    }

    // std::cout << "no path found between start " << start.x << "," << start.y << " and end " << end.x << "," << end.y << std::endl;

    return std::vector<sf::Vector2f>(); // return empty vector if no path found

}  


#endif