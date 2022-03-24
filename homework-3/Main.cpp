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
#include "ManhattanHeuristic.h"
#include "ManyPathsHeuristic.h"
#include "DijkstraHeuristic.h"

#include "from-homework-2/SteeringBehavior.h"
#include "PathFollower.h"

enum SearchType {
ASTAR,
DIJKSTRA,
MANYPATHS,
MANHATTAN,
MAZE
};


int demonstrateGraphSearch(enum SearchType searchType){

    std::string edgesFile;
    std::string nodesFile;


    std::cout << "Would you like to test on the big graph or the small graph? (b/s): ";
    char graphChoice;
    std::cin >> graphChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << std::endl;

    if (graphChoice == 'b' || graphChoice == 'B')
    {
        std::cout << "Loading the big graph..." << std::endl;
        edgesFile = "graphFiles/big_edges.csv";
        nodesFile = "graphFiles/big_nodes.csv";
    }else{
        std::cout << "Loading the small graph..." << std::endl;
        edgesFile = "graphFiles/edges.csv";
        nodesFile = "graphFiles/nodes.csv";
    }

    // check using stat to see if the files exist
    struct stat buffer;
    if (stat(edgesFile.c_str(), &buffer) != 0 || stat(nodesFile.c_str(), &buffer) != 0) {
        
        // if the folder doesn't exist, create it
        if (mkdir("graphFiles", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
        {
            std::cout << "Error creating graphFiles folder" << std::endl;
            return 1;
        }

        // Call randomDigraph.py to generate the files
        std::string command = "python3 scripts/generateDigraph.py";

        // Run the command. If it fails, print an error message and exit
        if(system(command.c_str()) != 0){
            return 1;
        }
    }

    // Read in a CSV list of edges and build a graph using Graph.h
    std::vector<GraphNode*> graph = buildGraph(edgesFile,nodesFile);

    int startID = 0;
    int goalID = graph.size() - 1;

    Heuristic* heuristic;

    auto start = std::chrono::high_resolution_clock::now();

    switch (  searchType)
    {
        case ASTAR:
        {
            std::cout << "Chosen Heuristic: A*" << std::endl;
            heuristic = new EuclideanHeuristic(graph[goalID]);
            break;
        }
        case DIJKSTRA:
        {
            std::cout <<"Chosen Heuristic: Dijkstra's (No heuristic)" << std::endl;
            heuristic = new DijkstraHeuristic(graph[goalID]);

            break;
        }
        case MANYPATHS:
        {
            std::cout <<"Chosen Heuristic: Many Paths" << std::endl;
            heuristic = new ManyPathsHeuristic(graph[goalID], graph);

            break;
        }
        case MANHATTAN:
        {
            std::cout <<"Chosen Heuristic: Manhattan" << std::endl;
            heuristic = new ManhattanHeuristic(graph[goalID]);

            break;
        }
        default:
        {
            std::cout << "An error occured in the switch statement" << std::endl;
            return 1;
        }
    }

    std::vector<GraphNode*> path = shortestPath(startID, goalID, graph, heuristic);

    // Get the end time
    auto end = std::chrono::high_resolution_clock::now();

    // Print out the time it took to execute
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds" << std::endl;

    std::cout << "Path from " << startID << " to " << goalID << ": ";

    if (path.empty())
    {
        std::cout << "No path found" << std::endl;
    } else{

        // Print out the path in reverse order
        for(int i = path.size()-1; i >= 1; i--){
            std::cout << path[i]->id << " -> ";
        }
        std::cout << path[0]->id << std::endl;
        std::cout << std::endl;

        // Print out the total cost of the path
        std::cout << "Total Cost: " << path.front()->g << std::endl;

    }

    // delete pointers
    for(auto node : graph){
        delete node;
    }
    graph.clear();
    path.clear();

    delete heuristic;

}

int demonstrateMazeNavigation(){

    int windowWidth = 1600;
    int windowHeight = 900;
    int targetFPS = 40;

    bool drawOverlay = false;

    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Demonstrating A* Maze Navigation",sf::Style::Close);

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

            else if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Space){
                    drawOverlay = !drawOverlay;

                }
            }
            
            else { 
                steeringBehavior->checkEvent(event);
            }
            
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

    // Translate the command line arguments into a search type enum
    SearchType searchType = MAZE;
    if (argc > 1)
    {
        if (strcasecmp(argv[1], "astar") == 0 || strcasecmp(argv[1],"a*")==0)
        {
            searchType = ASTAR;
        }
        else if (strcasecmp(argv[1], "dijkstra") == 0 || strcasecmp(argv[1], "dijkstras") == 0)
        {
            searchType = DIJKSTRA;
        }
        else if (strcasecmp(argv[1], "manyPaths") == 0 || strcasecmp(argv[1], "manyPath") == 0)
        {
            searchType = MANYPATHS;
        }
        else if (strcasecmp(argv[1], "manhattan") == 0)
        {
            searchType = MANHATTAN;
        }
    }

    if (searchType == MAZE)
    {
        return demonstrateMazeNavigation();
    }
    else
    {
        return demonstrateGraphSearch(searchType);
    }


}