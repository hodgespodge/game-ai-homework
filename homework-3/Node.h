#ifndef NODE_H
#define NODE_H

#include <vector>
#include <queue>

// Each 
class Node
{
    public:

        // priority queue of cost, node
        std::priority_queue<std::pair<int, Node*>> neighbors;
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
            neighbors.push(std::make_pair(cost, neighbor));
        }
};
#endif