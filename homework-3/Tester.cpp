#include <vector>
#include <iostream>
#include <functional>
#include <string>
#include <sys/stat.h>
#include <chrono>
#include <cctype>

#include "GraphNode.h"
#include "Graph.h"
#include "SearchFunctions.h"
#include "Heuristic.h"
#include "ManyPathsHeuristic.h"
#include "DijkstraHeuristic.h"
#include "EuclideanHeuristic.h"

enum SearchType {
    ASTAR,
    DIJKSTRA,
    MANYPATHS
};

int main(int argc, char *argv[])
{

    std::string edgesFile = "graphFiles/edges.csv";
    std::string nodesFile = "graphFiles/nodes.csv";

    // create a set of flags from the command line arguments
    std::set<std::string> flags;
    for (int i = 1; i < argc; i++)
    {
        flags.insert(argv[i]);
    }

    bool newGraph = false;
    // if "-n" or "--new" is in the flags, then create a new graph
    if (flags.find("-n") != flags.end() || flags.find("--new") != flags.end())
    {
        newGraph = true;
    }

    bool verbose = false;
    // if "-v" or "--verbose" is in the flags, then print out the graph
    if (flags.find("-v") != flags.end() || flags.find("--verbose") != flags.end())
    {
        verbose = true;
    }

    SearchType searchType = DIJKSTRA; // default search type
    if (flags.find("a") != flags.end() || flags.find("A") != flags.end())
    {
        searchType = ASTAR;
    }  else if(flags.find("m") != flags.end() || flags.find("M") != flags.end())
    {
        searchType = MANYPATHS;
    }

    // check using stat to see if the files exist
    struct stat buffer;
    if (newGraph || stat(edgesFile.c_str(), &buffer) != 0 || stat(nodesFile.c_str(), &buffer) != 0) {
        
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

    // Print out the graph if verbose is true
    // Print out each node and the edges to each neighbor

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

    int startID = 0;
    int goalID = graph.size() - 1;

    Heuristic* heuristic;

    // Get the start time
    // Must be included in the timing block incase heuristic performs preprocessing
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
