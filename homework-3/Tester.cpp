#include "GraphNode.h"
#include <vector>
#include "Graph.h"
#include <iostream>
#include "SearchFunctions.h"
#include <functional>
#include <string>
#include <sys/stat.h>


int main(int argc, char *argv[])
{

    // string = filename
    std::string graphFile = "graphFiles/digraph.csv";
    std::string numNodesFile = "graphFiles/numNodes.csv";

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

    // check using stat to see if the files exist
    struct stat buffer;
    if (newGraph || stat(graphFile.c_str(), &buffer) != 0 || stat(numNodesFile.c_str(), &buffer) != 0) {
        
        // Call randomDigraph.py to generate the files
        std::string command = "python3 scripts/randomDigraph.py";

        // Run the command. If it fails, print an error message and exit
        if(system(command.c_str()) != 0){
            std::cout << "Error: randomDigraph.py failed for some reason" << std::endl;
            std::cout << "Please make sure you have python3 installed" << std::endl;
            return 1;
        }

    }

    // Read in a CSV list of edges and build a graph using Graph.h
    std::vector<GraphNode*> graph = buildGraph(graphFile, numNodesFile);

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

    // Call the dijkstra search function
    // std::vector<GraphNode*> path = dijkstraPath(startID, goalID, graph);
    std::vector<GraphNode*> path = aStarPath(startID, goalID, graph);

    std::cout << "Path from " << startID << " to " << goalID << ": ";

    // Print out the path in reverse order
    for(int i = path.size()-1; i >= 0; i--){
        std::cout << path[i]->id << " ";
    }
    std::cout << std::endl;

}