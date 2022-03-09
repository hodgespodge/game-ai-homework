#include <SFML/Graphics.hpp>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <list>
#include <string>
#include <sys/stat.h>
#include <iostream>
#include <vector>

#include "MapDrawer.h"
#include "Graph.h"

int main(int argc, char *argv[])
{

    // set values here ------------------------------------------------------

    // int windowWidth = 640;
    // int windowHeight = 480;

    int windowWidth = 1920;
    int windowHeight = 1080;
    int targetFPS = 40;
    // int numThreads = 1;

    // end of user values ---------------------------------------------------

    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Demonstrating Steering Behavior");

    // sf::Texture texture;
    // if(!texture.loadFromFile("images/boid-sm.png")){
    //     return EXIT_FAILURE;
    // }

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

    std::vector<GraphNode*> graph = buildGraphFromMap(indoorMap,scale);

    // print graph size
    std::cout << "Graph size: " << graph.size() << std::endl;

    // initialize the map drawer
    MapDrawer mapDrawer(indoorMap, graph, scale);


    // std::list<Boid*> boids;

    // int numBreadCrumbs;
    // int numBoids;
    // bool drawBreadcrumbs;
    // bool fadeBreadcrumbs;
    // bool drawID;

    // SteeringBehavior* steeringBehavior = NULL;

    // // if second argument is "-v", use velocity matching
    // if(argc > 1 && strcmp(argv[1], "-v") == 0){
    //     steeringBehavior = new VelocityMatching(window);

    // } else if(argc > 1 && strcmp(argv[1], "-a") == 0){
    //     steeringBehavior = new ArriveAndAlign(window);

    // } else if(argc > 1 && strcmp(argv[1], "-w") == 0){
    //     steeringBehavior = new Wander(window);

    // } else if(argc > 1 && strcmp(argv[1], "-f") == 0){
    //     steeringBehavior = new Flocking(window,boids);
     
    // } else {
    //     steeringBehavior = new Flocking(window, boids);
    // }

    // printf("Using steering behavior: %s\n", steeringBehavior->getName().c_str());

    // numBoids = steeringBehavior->numBoids;
    // numBreadCrumbs = steeringBehavior->numBreadCrumbs;
    // drawBreadcrumbs = steeringBehavior->drawBreadcrumbs;
    // fadeBreadcrumbs = steeringBehavior->fadeBreadcrumbs;
    // drawID = steeringBehavior->drawID;

    // float windowSizeFactor = (window.getSize().x * window.getSize().y) / (float) IDEAL_WINDOW_SIZE;
    
    // float spriteScaleFactor = 2.0f*windowSizeFactor;
    // if (spriteScaleFactor > 1.5f) {
    //     spriteScaleFactor = 1.5f;
    // }else if (spriteScaleFactor < 0.5f){
    //     spriteScaleFactor = 0.5f;
    // }


    // initialize the boids
    // for(int i = 0; i < numBoids; i++){

    //     Boid * sprite = new Boid(numBreadCrumbs, spriteScaleFactor);

    //     sprite->setTexture(texture);
    //     sprite->setScale(2.0f*spriteScaleFactor, 2.0f*spriteScaleFactor);
    //     sprite->setPosition(rand() % (int)window.getSize().x, rand() % (int)window.getSize().y);
    //     sprite->linearVelocity = 0.05f*sf::Vector2f(rand() % 10 - 5, rand() % 10 - 5);
    //     sprite->setRotation(rand() % 360);
    //     sprite->setID(i);
    //     boids.push_back(sprite);

    // }

    // frames per update
    int updateFrames = 1000/targetFPS;

    // initialize a thread pool
    // BasicThreadPool * updater = NULL;
    // if (numThreads > 1){
    //     updater = new BasicThreadPool(8);
    // }
    
    while (window.isOpen())
    {

        // check for mouse events (used by arive and align)
        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
                window.close();

            // steeringBehavior->checkEvent(event);
            
        }

        // if enough time has passed, update the boids
        if (clock.getElapsedTime().asMilliseconds() > updateFrames)
        {
           
            // for(auto s : boids){
            //     steeringBehavior->updateSprite(*s, clock.getElapsedTime().asMilliseconds() );;
            // }
            
            // steeringBehavior->postUpdate();
            
            clock.restart();
        }

        // clear the window with white color
        window.clear(sf::Color::White);

        // if (drawBreadcrumbs)
        // {
        //     for(auto s : boids){
        //         s->drawBreadcrumbs(window, fadeBreadcrumbs);
        //         window.draw(*s);
        //     }
        // }

        // if (drawID)
        // {
        //     for(auto s : boids){
        //         s->drawID(window);
        //         window.draw(*s);
        //     }
        // }

        // else{
        //     for(auto s : boids){
        //         window.draw(*s);
        //     }
        // }

        // draw the map
        mapDrawer.drawMap(window);
        mapDrawer.drawOverlay(window);

        window.display();
    }
    

    return 0;
}

