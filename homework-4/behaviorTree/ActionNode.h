#ifndef ACTION_NODE_H
#define ACTION_NODE_H

#include "BehaviorTreeNode.h"
#include <unordered_map>

class ActionNode : public BTNode
{
public:

    ReturnType * actionResult; // Pointer to the result of the action (some exterior function)


    // Constructor
    ActionNode(int nodeID, ReturnType & actionResult)
    {
        this->nodeID = nodeID;
        this->actionResult = &actionResult;
    }

    // Destructor
    ~ActionNode()
    {
        for (auto child : children)
        {
            delete child;
        }
    }

    std::pair<int, ReturnType> evaluateNode()
    {
        std::pair<int, ReturnType> result;

        result.first = nodeID;

        if (*actionResult == IDLE){
            *actionResult = RUNNING;
        }

        result.second = *actionResult;

        return result;
        
    }

};

#endif