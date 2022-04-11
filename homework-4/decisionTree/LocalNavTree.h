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


// float target_range = 5;
float target_range = 10;

DTNode * createTree(ExposedVariables * variables) {

    DTNode *root = new DTNode(
        [](ExposedVariables & variables) -> bool {
            // std::cout << "in the root" << std::endl;
            return true;
        },
        *variables,
        -1
    );

    DTNode *within_range = new DTNode(
        [](ExposedVariables & variables) -> bool{
            // std::cout << "in the within_range" << std::endl;

            sf::Vector2f sprite_offset = variables.sprite->getPosition() - variables.local_target;

            // std::cout << "returning: (" << magnitude(sprite_offset) << " <= " << target_range << ")" << std::endl; 

            return (magnitude(sprite_offset) <= target_range);
        },
        *variables,
        -1
    );

    DTNode *not_within_range = new DTNode(
        [](ExposedVariables & variables) -> bool{
            // std::cout << "in the not_within_range" << std::endl;
            sf::Vector2f sprite_offset = variables.sprite->getPosition() - variables.local_target;

            // std::cout << "returning: (" << magnitude(sprite_offset) << " > " << target_range << ")" << std::endl;

            return (magnitude(sprite_offset) > target_range);

        },
        *variables,
        -1
    );

    DTNode *if_local_path_empty = new DTNode(
        [](ExposedVariables & variables) -> bool{
            // std::cout << "in the if_local_path_empty" << std::endl;

            return variables.localPath.size() <= 1;
            
        },
        *variables,
        -1
    );

    DTNode *if_global_path_empty = new DTNode(
        [](ExposedVariables & variables) -> bool{
            // std::cout << "in the if_global_path_empty" << std::endl;

            // std::cout << "returning variables.globalPath.size()" << variables.globalPath.size() << std::endl;

            return variables.globalPath.size() == 0;
        },
        *variables,
        0
    );

    DTNode *if_not_global_path_empty = new DTNode(
        [](ExposedVariables & variables) -> bool{
            // std::cout << "in the if_not_global_path_empty" << std::endl;
            return variables.globalPath.size() != 0;
        },
        *variables,
        1
    );

    DTNode *if_not_local_path_empty = new DTNode(
        [](ExposedVariables & variables) -> bool{
            // std::cout << "in the if_not_local_path_empty" << std::endl;
            return variables.localPath.size() > 1;
            
        },
        *variables,
        2
    );

    DTNode *if_local_path_not_set_OR_enemy_in_room = new DTNode(
        [](ExposedVariables & variables) -> bool{
            // std::cout << "in the if_local_path_not_set_OR_enemy_in_room" << std::endl;
            if (variables.localPath.size() == 0) {
                // std::cout << "local path is empty" << std::endl;
                return true;
            }
            if (variables.current_room.contains(variables.enemy_position.x, variables.enemy_position.y)) {
                std::cout << "enemy in room" << std::endl;
                return true;
            }
            return false;
        },
        *variables,
        3
    );
  
    DTNode *if_local_path_set_AND_enemy_not_in_room = new DTNode(
        [](ExposedVariables & variables) -> bool{
            // std::cout << "in the if_local_path_set_AND_enemy_not_in_room" << std::endl;
            return variables.localPath.size() > 0 && !variables.current_room.contains(variables.enemy_position.x, variables.enemy_position.y);
        },
        *variables,
        4
    );


    root->AddChild(within_range);
        within_range->AddChild(if_local_path_empty); 
            if_local_path_empty->AddChild(if_global_path_empty); // leaf 0
            if_local_path_empty->AddChild(if_not_global_path_empty); // leaf 1

        within_range->AddChild(if_not_local_path_empty); // leaf 2

    root->AddChild(not_within_range);
        not_within_range->AddChild(if_local_path_not_set_OR_enemy_in_room); // leaf 3
        not_within_range->AddChild(if_local_path_set_AND_enemy_not_in_room); // leaf 4

    return root;

}
#endif


