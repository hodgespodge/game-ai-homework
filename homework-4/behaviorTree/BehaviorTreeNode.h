#ifndef BEHAVIOR_TREE_NODE_H
#define BEHAVIOR_TREE_NODE_H

#include <vector>

enum ReturnType {
    SUCCESS,
    FAILURE,
    RUNNING,
    IDLE
};



class BTNode
{
public:

    int nodeID;

    virtual std::pair<int, ReturnType> evaluateNode() = 0;

    BTNode* parent;
    std::vector<BTNode*> children;
};


#endif