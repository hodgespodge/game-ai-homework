#include <SFML/Graphics.hpp>
#include <math.h>
#include <stdio.h>
#include <string.h>
// #include <list>
#include <string>
#include <sys/stat.h>
#include <iostream>
#include <vector>

#include "MapDrawer.h"
#include "Graph.h"
#include "from-homework-2/Boid.h"
#include "EuclideanHeuristic.h"
#include "from-homework-2/SteeringBehavior.h"
#include "PathFollower.h"

int main(int argc, char *argv[])
{

    int windowWidth = 1600;
    int windowHeight = 900;
    int targetFPS = 40;

    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Demonstrating Steering Behavior");

    std::string indoorMapFile = "graphFiles/indoorMap.csv";

    // check using stat to see if the files exist
    struct stat buffer;
    if (stat(indoorMapFile.c_str(), &buffer) != 0)
    {
        std::cout << "Error: " << indoorMapFile << " does not exist" << std::endl;
        return 1;
    }

    std::vector<std::vector<std::string>> indoorMap = buildMap(indoorMapFile);

    // Get map size
    int mapWidth = indoorMap[0].size();
    int mapHeight = indoorMap.size();

    // Whichever is larger is the map size
    int mapSize = indoorMap[0].size() > indoorMap.size() ? indoorMap[0].size() : mapHeight;

    // Whichever is smaller is the window size
    int windowSize = windowWidth > windowHeight ? windowWidth : windowHeight;

    float scale = windowSize/mapSize;

    buildGraphFromMapReturn graph_rooms = buildGraphFromMap(indoorMap, scale);

    std::vector<GraphNode*> graph = graph_rooms.graph;
    std::vector<room> rooms = graph_rooms.rooms;

    // print graph size
    std::cout << "Graph size: " << graph.size() << std::endl;

    // initialize the map drawer
    MapDrawer mapDrawer(indoorMap, graph, rooms , scale);

    SteeringBehavior* steeringBehavior = new PathFollower(window, graph, rooms);

    int numBreadCrumbs = 30;
    bool drawBreadcrumbs = true;
    bool fadeBreadcrumbs = true;
    bool drawID = false;

    Boid* sprite = new Boid(numBreadCrumbs, 1.0);


    sf::Texture texture;
    if(!texture.loadFromFile("images/boid-sm.png")){
        return EXIT_FAILURE;
    }

    sprite->setTexture(texture);
    // sprite->setScale(2.0f*spriteScaleFactor, 2.0f*spriteScaleFactor);
    // sprite->setPosition(rand() % (int)window.getSize().x, rand() % (int)window.getSize().y);
    sprite->setPosition(100, 100);
    sprite->setID(0);


    // frames per update
    int updateFrames = 1000/targetFPS;

    
    while (window.isOpen())
    {

        // check for mouse events (used by arive and align)
        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
                window.close();

            steeringBehavior->checkEvent(event);
            
        }

        // if enough time has passed, update the boids
        if (clock.getElapsedTime().asMilliseconds() > updateFrames)
        {
           
            steeringBehavior->updateSprite(*sprite, clock.getElapsedTime().asMilliseconds() );
            
            clock.restart();
        }

        // clear the window with white color
        window.clear(sf::Color::White);
        mapDrawer.drawFloors(window);
        mapDrawer.drawMap(window);
        // mapDrawer.drawOverlay(window);

        if (drawBreadcrumbs)
        {
            sprite->drawBreadcrumbs(window, fadeBreadcrumbs);    
        }

        if (drawID)
        {
            sprite->drawID(window);
            window.draw(*sprite);
        }

        else{
            window.draw(*sprite);
        }

        window.display();
    }

    // delete anything that needs to be deleted
    delete sprite;
    delete steeringBehavior;

    return 0;
}

