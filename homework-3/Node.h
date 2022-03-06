#ifndef NODE_H
#define NODE_H

#include <vector>
// #include <queue>
#include <set>

// Each 
class Node
{
    public:

        // priority queue of cost, node
        std::set<std::pair<int, Node*>> neighbors;
        int id;

        // Constructor
        Node(int id){
            this->id = id;
        }
        Node(){
            this->id = -1;
        }
        
        // Destructor
        ~Node();

        // Add a neighbor to the node
        void addNeighbor(Node* neighbor, int cost){
            neighbors.emplace(std::make_pair(cost, neighbor));
        }

        // // Get the neighbors of the node as a vector of pointers
        // std::vector<std::pair<int, Node*>> neighborsAsVector(){
        //     std::vector<std::pair<int, Node*>> neighborsVector;
        //     for(auto neighbor : neighbors){
        //         neighborsVector.push_back(neighbor);
        //     }

        //     return neighborsVector;
        // }
};
#endif