#include <vector>
#include <iostream>
#include <functional>
#include <string>
#include <sys/stat.h>
#include <chrono>
#include <cctype>
#include <set>
// #include <SFML/Graphics.hpp>
#include <iomanip>


#include "Graph.h"

int main(int argc, char *argv[]){
    std::string indoorMapFile = "graphFiles/indoorMap.csv";

    // create a set of flags from the command line arguments
    std::set<std::string> flags;
    for (int i = 1; i < argc; i++)
    {
        flags.insert(argv[i]);
    }


    // check using stat to see if the files exist
    struct stat buffer;
    if (stat(indoorMapFile.c_str(), &buffer) != 0)
    {
        std::cout << "Error: " << indoorMapFile << " does not exist" << std::endl;
        return 1;
    }
    
    std::vector<std::vector<std::string>> indoorMap = buildMap(indoorMapFile);

    // print out the map
    for (int i = 0; i < indoorMap.size(); i++)
    {
        for (int j = 0; j < indoorMap[i].size(); j++)
        {

            std::cout << std::setw(2) << indoorMap[i][j];
        }
        std::cout << std::endl;
    }

    // print out the map size
    std::cout << "Map size: " << indoorMap.size() << " x " << indoorMap[0].size() << std::endl;

    float scale = 1.0f;

    std::vector<GraphNode*> graph = buildGraphFromMap(indoorMap,scale);

    // print graph size
    std::cout << "Graph size: " << graph.size() << std::endl;

    // print out the graph nodes and their neighbors
    bool verbose = true;
    if (verbose){

        int nodesPrinted = 0;
      
        for(int linesPrinted = 0; nodesPrinted < graph.size() && linesPrinted < 50; nodesPrinted++){
            // Print out the node
            std::cout << "Node " << graph[nodesPrinted]->id << ": ";
            std::cout << std::endl;
            linesPrinted++;

            // For each neighbor, print out the neighbor and the cost of the edge
            for(auto neighbor : graph[nodesPrinted]->neighbors){
                std::cout << "    " << neighbor.second->id << ": " << neighbor.first << std::endl;
                linesPrinted++;
            }
        }

        if (nodesPrinted < graph.size()/2){
            nodesPrinted = graph.size() - nodesPrinted;
            printf("\n.\n.\n.\n.\n\n");
        }

        for(; nodesPrinted < graph.size(); nodesPrinted++){
            // Print out the node
            std::cout << "Node " << graph[nodesPrinted]->id << ": ";
            std::cout << std::endl;

            // For each neighbor, print out the neighbor and the cost of the edge
            for(auto neighbor : graph[nodesPrinted]->neighbors){
                std::cout << "    " << neighbor.second->id << ": " << neighbor.first << std::endl;
            }
        }

    }

    // delete pointers
    for(auto node : graph){
        delete node;
    }
    // graph.clear();
    // path.clear();

    // delete heuristic;

}