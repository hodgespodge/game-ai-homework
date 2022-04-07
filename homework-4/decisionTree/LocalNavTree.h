#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include "DecisionTreeNode.h"

typedef std::unordered_map<std::string, std::any> VariableMap;

VariableMap * initLocals(){
    VariableMap * locals = new VariableMap();

    // Whenever you add a new variable to the tree, you must add it to the locals map
    locals->insert({"boid", NULL});
    locals->insert({"local_target", NULL});
    locals->insert({"path", NULL}); // path is a vector of GraphNodes
    locals->insert({"current_room", NULL});
    locals->insert({"enemy_position", NULL});
    locals->insert({"obstacles", NULL});

    return locals;
}

DTNode * createTree(std::unordered_map<std::string, std::any> * locals ){

    DTNode *root = new DTNode(
        [](VariableMap &variables) -> bool {
            return true;
        },
        locals,
        -1
    );

    DTNode *node1 = new DTNode(
        [](VariableMap &variables) -> bool {
            return std::any_cast<int>(variables["x"]) < 0;
        },
        locals,
        1
    );

    DTNode *node2 = new DTNode(
        [](VariableMap &variables) -> bool {
            return std::any_cast<int>(variables["x"]) >= 0;
        },
        locals,
        2
    );

    root->AddChild(node1);
    root->AddChild(node2);

    return root;
}

#endif // DECISIONTREE_H