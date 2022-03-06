#ifndef SEARCHNODE_H
#define SEARCHNODE_H

#include "Node.h"

// subclass of Node
class SearchNode : public Node
{
public:

    float g; //The pathcost from the start node to this node
    float h; //The heuristic cost from this node to the goal node
    SearchNode* parent; //The parent of this node in the search tree

    // Construct SearchNode from an existing Node
    SearchNode(Node* node,float g, float h) : Node(node->id)
    {
        this->g = g;
        this->h = h;
        parent = NULL;
    }

    float f()
    {
        return g + h;
    }

};

#endif