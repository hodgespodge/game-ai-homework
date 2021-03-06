#include <SFML/Graphics.hpp>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <chrono>
#include <string>
#include <sys/stat.h>
#include <iostream>
#include <vector>

#include "MapDrawer.h"
#include "Graph.h"
#include "from-homework-2/Boid.h"

#include "EuclideanHeuristic.h"

#include "from-homework-2/SteeringBehavior.h"
#include "CatAI.h"
#include "PathFollower.h"

#include "decisionTree/NavTree.h"


int demonstrateMazeNavigation(bool new_map){

    int windowWidth = 1600;
    int windowHeight = 900;
    int targetFPS = 40;

    bool drawOverlay = false;

    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Demonstrating A* Maze Navigation",sf::Style::Close);

    std::string indoorMapFile = "graphFiles/indoorMap.csv";

    // check using stat to see if the files exist
    struct stat buffer;
    if (new_map || stat(indoorMapFile.c_str(), &buffer) != 0)
    {
        // std::cout << "Error: " << indoorMapFile << " does not exist" << std::endl;

        std::cout << "Generating new map..." << std::endl;

        // call the generateIndoorMaze script to generate the map
        std::string command = "python3 scripts/generateIndoorMaze.py";
        system(command.c_str());

    }

    IndoorMap indoorMap = buildMap(indoorMapFile);

    // Get map size
    int mapWidth = indoorMap[0].size();
    int mapHeight = indoorMap.size();

    // Whichever is larger is the map size
    int mapSize = indoorMap[0].size() > indoorMap.size() ? indoorMap[0].size() : mapHeight;

    // Whichever is smaller is the window size
    int windowSize = windowWidth > windowHeight ? windowWidth : windowHeight;

    float scale = windowSize/mapSize;

    auto graph_rooms = buildGraphFromMap(indoorMap, scale);

    std::vector<GraphNode*> graph = graph_rooms.first;
    std::vector<room*>  rooms = graph_rooms.second;

    sf::Sprite * cheese = new sf::Sprite();
    sf::Texture cheeseTexture;
    if (!cheeseTexture.loadFromFile("images/cheese.png"))
    {
        std::cout << "Error: cheese.png not found" << std::endl;
        return -1;
    }
    cheese->setTexture(cheeseTexture);

    // make the cheese transparent initially
    cheese->setColor(sf::Color(255, 255, 255, 0));


    Boid* enemy = new Boid(5, 1.0);
    sf::Texture catTexture;
    if (!catTexture.loadFromFile("images/cat.png"))
    {
        std::cout << "Error: cat.png not found" << std::endl;
        return -1;
    }
    enemy->setTexture(catTexture);
    enemy->setPosition(sf::Vector2f(windowWidth*2/3, windowHeight *2/3));

    // initialize the map drawer
    MapDrawer mapDrawer(indoorMap , graph, rooms, scale);

    SteeringBehavior* steeringBehavior = new PathFollower(window, graph, rooms, *cheese, *enemy);

    

    int numBreadCrumbs = 30;
    bool drawBreadcrumbs = true;
    bool fadeBreadcrumbs = true;
    bool drawID = false;

    Boid* sprite = new Boid(numBreadCrumbs, 1.0);

    CatAI* catAI = new CatAI(window, graph, rooms, *sprite);

    sf::Texture texture;
    if(!texture.loadFromFile("images/mouse.png")){
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

            if (event.type == sf::Event::Closed){
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Space){
                    drawOverlay = !drawOverlay;
                } 
                else if (event.key.code == sf::Keyboard::LShift ){
                    steeringBehavior->shiftPressed = true;
                    // std::cout << "shift pressed" << std::endl;
                }
            }
            else if (event.type == sf::Event::KeyReleased){
                if (event.key.code == sf::Keyboard::LShift){
                    steeringBehavior->shiftPressed = false;
                    // std::cout << "shift released" << std::endl;
                }
            }
            else { 
                steeringBehavior->checkEvent(event);
            }
            
        }

        // if enough time has passed, update the boids
        if (clock.getElapsedTime().asMilliseconds() > updateFrames)
        {
           
            steeringBehavior->updateSprite(*sprite, clock.getElapsedTime().asMilliseconds());
            catAI->updateSprite(*enemy, clock.getElapsedTime().asMilliseconds());

            clock.restart();
        }

        // clear the window with white color
        window.clear(sf::Color::White);
        mapDrawer.drawFloors(window);
        mapDrawer.drawMap(window);
        mapDrawer.drawObstacles(window);

        if (drawOverlay){
            mapDrawer.drawOverlay(window);
        }

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
            window.draw(*cheese);
            window.draw(*enemy);
        }

        window.display();
    }

    // delete anything that needs to be deleted
    delete sprite;
    delete steeringBehavior;

    return 0;
}


    
int main(int argc, char *argv[])
{
    // if get "-n" argument, generate a new map
    bool new_map = false;
    if (argc > 1){
        if (strcmp(argv[1], "-n") == 0){
            new_map = true;
        }
    }

    return demonstrateMazeNavigation(new_map);

}