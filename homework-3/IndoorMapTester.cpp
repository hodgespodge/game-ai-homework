#include <vector>
#include <iostream>
#include <functional>
#include <string>
#include <sys/stat.h>
#include <chrono>
#include <cctype>
#include <set>
#include <SFML/Graphics.hpp>

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
    
    std::vector<std::vector<char>> indoorMap = buildMap(indoorMapFile);

    // print out the map
    for (int i = 0; i < indoorMap.size(); i++)
    {
        for (int j = 0; j < indoorMap[i].size(); j++)
        {
            std::cout << indoorMap[i][j];
        }
        std::cout << std::endl;
    }

    std::vector<GraphNode*> graph = buildGraphFromMap();


    // delete pointers
    for(auto node : graph){
        delete node;
    }
    // graph.clear();
    // path.clear();

    // delete heuristic;

}