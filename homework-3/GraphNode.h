#ifndef GRAPHNODE_H
#define GRAPHNODE_H
#include <vector>
#include <set>

#include <limits>

const int INF = std::numeric_limits<int>::max();

// Each 
class GraphNode
{
    public:

        // ordered set of <int cost, Node* neighbor>
        std::set<std::pair<int, GraphNode*>> neighbors;
        int id;
        float g; //The pathcost from the start node to this node
        float h; //The heuristic cost from this node to the goal node
        GraphNode* parent; //The parent of this node in the search tree
        bool visited; //Whether or not this node has been visited
        int x;
        int y;

        // Constructor
        GraphNode(int id, int x, int y){
            this->id = id;
            this->x = x;
            this->y = y;
            this->g = INF;
            this->h = INF;
            parent = NULL;
            visited = false;
        }

        // Add a neighbor to the node
        void addNeighbor(GraphNode* neighbor, int cost){
            neighbors.emplace(std::make_pair(cost, neighbor));
        }

        // Remove a neighbor from the node
        void removeNeighbor(int neighborID){
            for(auto it = neighbors.begin(); it != neighbors.end(); ++it){
                if(it->second->id == neighborID){
                    neighbors.erase(it);
                    return;
                }
            }
            std::cout << "Neighbor not found" << std::endl;
        }

        // calling the destructor will delete the entire graph I believe
        ~GraphNode(){
            neighbors.clear();
            parent = NULL;
            
        }

        float f()
        {
            return g + h;
        }
};


#endif