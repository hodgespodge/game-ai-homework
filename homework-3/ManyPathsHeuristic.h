#ifndef MANYPATHSHEURISTIC_H
#define MANYPATHSHEURISTIC_H

#include "GraphNode.h"
#include "Heuristic.h"

class ManyPathsHeuristic : public Heuristic
{
public:

    // std::vector<GraphNode*> nodeGraph;
    float average_edge_cost;

    ManyPathsHeuristic(GraphNode* goal, std::vector<GraphNode*> nodeGraph) : Heuristic(goal)
    {
        // calculate the average edge cost between all nodes
        float total_path_cost = 0;
        int num_edges = 0;

        for (int i = 0; i < nodeGraph.size(); i++)
        {

            for(auto it = nodeGraph[i]->neighbors.begin(); it != nodeGraph[i]->neighbors.end(); ++it){
                total_path_cost += it->first;
                num_edges++;
            }
            if (num_edges > 1000){
                break;
            }
        }

        average_edge_cost = total_path_cost / num_edges;
    }
    
    // favor nodes that have more neighbors
    float f(GraphNode* node){
        
        return average_edge_cost/(1+node->neighbors.size());
    }
    ~ManyPathsHeuristic(){
    }

};


#endif