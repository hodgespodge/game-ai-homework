#ifndef RANDOM_SEQUENCE_H
#define RANDOM_SEQUENCE_H

#include "BehaviorTreeNode.h"
#include <algorithm>

class RandomSequence : public BTNode
{
public:

    // Constructor
    RandomSequence(int nodeID)
    {
        this->nodeID = nodeID;
    }

    // Destructor
    ~RandomSequence()
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

        std::random_shuffle(children.begin(), children.end());

        for (auto child : children)
        {
            result = child->evaluateNode();
            if (result.second == FAILURE || result.second == RUNNING)
            {
                return result; // First successful OR running node
            }
        }

        return {nodeID,SUCCESS}; // Last successful node
    }

};

#endif