#ifndef EUCLIDEANHEURISTIC_H
#define EUCLIDEANHEURISTIC_H

# include "Heuristic.h"
# include "GraphNode.h"
# include <cmath>

class EuclideanHeuristic: public Heuristic
{
public:
    EuclideanHeuristic(GraphNode* goal) : Heuristic(goal) {
    };

    float f(GraphNode* node)
    {

        int x_diff = node->x - goal->x;
        int y_diff = node->y - goal->y;
        
        return sqrt(x_diff*x_diff + y_diff*y_diff);
    }

    ~EuclideanHeuristic()
    {
    }
};

#endif