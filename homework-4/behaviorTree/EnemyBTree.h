#ifndef ENEMYBTREE_H
#define ENEMYBTREE_H

#include "BehaviorTreeNode.h"
#include "Selector.h"
#include "Sequence.h"
#include "ActionNode.h"
#include "RandomSequence.h"

BTNode * createTree(std::unordered_map<int, ReturnType *> * blackboard){

    BTNode * root = new Selector(0);
    
        BTNode * huntSequence = new Sequence(0);
        root -> children.push_back(huntSequence);

            BTNode * findMouse = new ActionNode(1, *blackboard->at(1));
            huntSequence -> children.push_back(findMouse);
            BTNode * chaseMouse = new ActionNode(2, *blackboard->at(2));
            huntSequence -> children.push_back(chaseMouse);

        BTNode * wanderSequence = new RandomSequence(0);
        root -> children.push_back(wanderSequence);

            BTNode * dance = new ActionNode(3, *blackboard->at(3));
            wanderSequence -> children.push_back(dance);
            BTNode * wanderToRoom = new ActionNode(4, *blackboard->at(4));
            wanderSequence -> children.push_back(wanderToRoom);

    return root;

}

#endif