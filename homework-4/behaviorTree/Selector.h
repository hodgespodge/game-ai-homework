#ifndef SELECTOR_H
#define SELECTOR_H

#include "BehaviorTreeNode.h"

class Selector : public BTNode
{
public:

    // Constructor
    Selector(int nodeID)
    {
        this->nodeID = nodeID;
    }

    // Destructor
    ~Selector()
    {
        for (auto child : children)
        {
            delete child;
        }
    }

    std::pair<int, ReturnType> evaluateNode()
    {
        std::pair<int, ReturnType> result;

        if (children.size() == 0)
        {
            return {nodeID, ReturnType::FAILURE};
        }

        for (auto child : children)
        {
            result = child->evaluateNode();
            if (result.second == SUCCESS || result.second == RUNNING)
            {
                return result; // First successful OR running node
            }

        }
        return {nodeID, FAILURE}; // Last failing node
    }
    

};
#endif