#ifndef MANHATTANHEURISTIC_H
#define MANHATTANHEURISTIC_H

# include "Heuristic.h"
# include "GraphNode.h"
# include <cmath>

class ManhattanHeuristic: public Heuristic
{
public:
    ManhattanHeuristic(GraphNode* goal) : Heuristic(goal) {
    };

    float h(GraphNode* node)
    {

        int x_diff = node->x - goal->x;
        int y_diff = node->y - goal->y;

        return abs(x_diff) + abs(y_diff);

    }

    ~ManhattanHeuristic()
    {
    }
};

#endif