#ifndef DIJKSTRAHEURISTIC_H
#define DIJKSTRAHEURISTIC_H

# include "Heuristic.h"
# include "GraphNode.h"

class DijkstraHeuristic: public Heuristic
{
public:
    DijkstraHeuristic(GraphNode* goal) : Heuristic(goal) {
    };

    float h(GraphNode* node)
    {
        return 0;
    }

    ~DijkstraHeuristic()
    {
    }
};

#endif