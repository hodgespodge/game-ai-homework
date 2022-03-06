#ifndef EUCLIDEANHEURISTIC_H
#define EUCLIDEANHEURISTIC_H

# include "Heuristic.h"
# include "GraphNode.h"

class EuclideanHeuristic: public Heuristic
{
public:
    EuclideanHeuristic(GraphNode* goal) : Heuristic(goal) {
    };

    float f(GraphNode* node)
    {
        return 0;
    }

    ~EuclideanHeuristic()
    {
    }
};

#endif