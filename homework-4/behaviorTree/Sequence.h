#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "BehaviorTreeNode.h"

class Sequence : public BTNode
{
public:

    // Constructor
    Sequence(int nodeID)
    {
        this->nodeID = nodeID;
    }

    // Destructor
    ~Sequence()
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
            if (result.second == FAILURE || result.second == RUNNING)
            {
                return result; // First failing OR running node
            }

        }
        return {nodeID, SUCCESS}; // Last successful node
    }

};

#endif