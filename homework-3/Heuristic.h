#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "GraphNode.h"

// abstract class representing a heuristic function
class Heuristic
{
public:

    GraphNode* goal;

    // constructor
    Heuristic(GraphNode* goal)
    {
        this->goal = goal;
    }

    // returns the heuristic value of the node
    virtual float h(GraphNode* node) = 0;

    // destructor
    virtual ~Heuristic(){
        goal = NULL;
    }
};

#endif