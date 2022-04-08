#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include "DecisionTreeNode.h"
#include "SFML/Graphics.hpp"
#include "from-homework-2/Boid.h"
#include "from-homework-2/HelperFunctions.h"

typedef std::unordered_map<std::string, std::any> VariableMap;

VariableMap * initLocals(){
    VariableMap * locals = new VariableMap();

    // Whenever you add a new variable to the tree, you must add it to the locals map
    locals->insert({"sprite", NULL});
    locals->insert({"local_target", NULL});
    locals->insert({"path", NULL}); // path is a vector of GraphNodes
    locals->insert({"local_path", NULL}); // local_path is a vector of sf::Vector2f
    locals->insert({"current_room", NULL});
    locals->insert({"enemy_position", NULL});
    locals->insert({"obstacles", NULL});

    return locals;
}

float doorInnerRadius = 10;
float doorOuterRadius = 100;

DTNode * createTree(VariableMap * locals ){

    DTNode *root = new DTNode(
        [](VariableMap &variables) -> bool {
            return true;
        },
        locals,
        -1
    );

    DTNode *within_range = new DTNode(
        [](VariableMap &variables) -> bool {
            Boid sprite = std::any_cast<Boid>(variables["sprite"]);
            sf::Vector2f target = std::any_cast<sf::Vector2f>(variables["local_target"]);
            float distance = sqrt(pow(sprite.getPosition().x - target.x, 2) + pow(sprite.getPosition().y - target.y, 2));
            return distance < doorInnerRadius;
        },
        locals,
        -1
    );
    root->AddChild(within_range);

    // TODO: verify
    DTNode *if_final_node = new DTNode( 
        [](VariableMap &variables) -> bool {
            GraphNode * nextNode = std::any_cast<GraphNode*>(variables["nextNode"]);
            return nextNode == NULL;
        },
        locals,
        0 // get new path + new target
    );
    within_range->AddChild(if_final_node);

    // TODO: verify
    DTNode *if_not_final_node = new DTNode(
        [](VariableMap &variables) -> bool {
            GraphNode * nextNode = std::any_cast<GraphNode*>(variables["nextNode"]);
            return nextNode != NULL;
        },
        locals,
        1 // get next target
    );
    within_range->AddChild(if_not_final_node);
    

    DTNode *not_within_range = new DTNode(
        [](VariableMap &variables) -> bool { 
            Boid sprite = std::any_cast<Boid>(variables["sprite"]);
            sf::Vector2f target = std::any_cast<sf::Vector2f>(variables["local_target"]);
            float distance = sqrt(pow(sprite.getPosition().x - target.x, 2) + pow(sprite.getPosition().y - target.y, 2));
            return distance > doorInnerRadius;
        }, 
        locals,
        -1
    );
    root->AddChild(not_within_range);

    DTNode *if_local_path_not_set_OR_enemy_in_room = new DTNode(
        [](VariableMap &variables) -> bool {
            std::vector<sf::Vector2f> * local_path = std::any_cast<std::vector<sf::Vector2f>*>(variables["local_path"]);
            sf::Vector2f enemy_position = std::any_cast<sf::Vector2f>(variables["enemy_position"]);
            room current_room = std::any_cast<room>(variables["current_room"]);
            return local_path == NULL || current_room.contains(enemy_position); 
        },
        locals,
        2 // Calculate a new local_path
    );
    not_within_range->AddChild(if_local_path_not_set_OR_enemy_in_room);

    DTNode *if_local_path_set_AND_enemy_not_in_room = new DTNode(
        [](VariableMap &variables) -> bool {
            std::vector<sf::Vector2f> * local_path = std::any_cast<std::vector<sf::Vector2f>*>(variables["local_path"]);
            sf::Vector2f enemy_position = std::any_cast<sf::Vector2f>(variables["enemy_position"]);
            room current_room = std::any_cast<room>(variables["current_room"]);
            return local_path != NULL && !current_room.contains(enemy_position); 
        },
        locals,
        3 // Follow local_path
    );

    return root;
}

#endif // DECISIONTREE_H