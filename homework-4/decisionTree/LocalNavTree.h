#ifndef DECISIONTREE2_H
#define DECISIONTREE2_H

// #include "DecisionTreeNode.h"
#include "SFML/Graphics.hpp"
#include "../from-homework-2/Boid.h"
#include "../from-homework-2/HelperFunctions.h"
#include "../GraphNode.h"
#include "../Room.h"
#include "ExposedVariables.h"
#include "DecisionTreeNode.h"


float target_range = 10;

DTNode * createTree(ExposedVariables * variables) {

    DTNode *root = new DTNode(
        [](ExposedVariables & variables) -> bool {
            std::cout << "in the root" << std::endl;
            return true;
        },
        *variables,
        -1
    );

    DTNode *within_range = new DTNode(
        [](ExposedVariables & variables) -> bool{

            sf::Vector2f sprite_offset = variables.sprite->getPosition() - variables.local_target;

            return (magnitude(sprite_offset) <= target_range);
        },
        *variables,
        -1
    );


    root->AddChild(within_range);

    DTNode *if_final_node = new DTNode(
        [](ExposedVariables & variables) -> bool{
            return variables.localPath.size() <= 1;
            
        },
        *variables,
        0
    );

    within_range->AddChild(if_final_node);

    DTNode *if_not_final_node = new DTNode(
        [](ExposedVariables & variables) -> bool{
            return variables.localPath.size() > 1;
            
        },
        *variables,
        1
    );

    within_range->AddChild(if_not_final_node);

    DTNode *not_within_range = new DTNode(
        [](ExposedVariables & variables) -> bool{
            sf::Vector2f sprite_offset = variables.sprite->getPosition() - variables.local_target;

            return (magnitude(sprite_offset) > target_range);

        },
        *variables,
        -1
    );

    root->AddChild(not_within_range);

    DTNode *if_local_path_not_set_OR_enemy_in_room = new DTNode(
        [](ExposedVariables & variables) -> bool{
            if (variables.localPath.size() == 0) {
                return true;
            }
            if (variables.current_room.contains(variables.enemy_position.x, variables.enemy_position.y)) {
                return true;
            }
            return false;
        },
        *variables,
        2
    );
  
    not_within_range->AddChild(if_local_path_not_set_OR_enemy_in_room);

    DTNode *if_local_path_set_AND_enemy_not_in_room = new DTNode(
        [](ExposedVariables & variables) -> bool{
            return variables.localPath.size() > 0 && !variables.current_room.contains(variables.enemy_position.x, variables.enemy_position.y);
        },
        *variables,
        3
    );

    not_within_range->AddChild(if_local_path_set_AND_enemy_not_in_room);

    return root;

}
#endif


