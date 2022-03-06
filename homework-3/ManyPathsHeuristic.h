#ifndef MANYPATHSHEURISTIC_H
#define MANYPATHSHEURISTIC_H

#include "GraphNode.h"
#include "Heuristic.h"

class ManyPathsHeuristic : public Heuristic
{
public:

    std::vector<GraphNode*> nodeGraph;
    std::vector<std::vector<GraphNode*>> clusters;

    ManyPathsHeuristic(GraphNode* goal, std::vector<GraphNode*> nodeGraph) : Heuristic(goal)
    {
        this->nodeGraph = nodeGraph;
    }
    
    float f(GraphNode* node){
        
        return 10/(1+node->neighbors.size());
    }
    ~ManyPathsHeuristic(){
        nodeGraph.clear();
        clusters.clear();
    }

};


#endif