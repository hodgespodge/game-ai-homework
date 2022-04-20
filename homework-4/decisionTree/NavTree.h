#ifndef NAVTREE_H
#define NAVTREE_H

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

    DTNode *is_paused = new DTNode(
        [](ExposedVariables & variables) -> bool {
            // std::cout << "in the is_paused" << std::endl;
            return variables.is_paused;
        },
        *variables,
        5
    );

    DTNode *is_not_paused = new DTNode(
        [](ExposedVariables & variables) -> bool {
            // std::cout << "in the is_not_paused" << std::endl;
            return !variables.is_paused;
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

    DTNode *if_path_empty = new DTNode(
        [](ExposedVariables & variables) -> bool{
            // std::cout << "in the if_path_empty" << std::endl;

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

    DTNode *if_not_path_empty = new DTNode(
        [](ExposedVariables & variables) -> bool{
            // std::cout << "in the if_not_path_empty" << std::endl;
            return variables.localPath.size() > 1;
            
        },
        *variables,
        2
    );

    DTNode *if_path_not_set = new DTNode(
        [](ExposedVariables & variables) -> bool{
            // std::cout << "in the if_path_not_set_OR_enemy_in_room" << std::endl;
            
            return variables.localPath.size() == 0;
            
        },
        *variables,
        3
    );
  
    DTNode *if_path_set = new DTNode(
        [](ExposedVariables & variables) -> bool{
            // std::cout << "in the if_path_set_AND_enemy_not_in_room" << std::endl;
            return variables.localPath.size() != 0;
        },
        *variables,
        -1
    );

    DTNode *if_enemy_in_room = new DTNode(
        [](ExposedVariables & variables) -> bool{
            // std::cout << "in the if_enemy_in_room" << std::endl;
            return variables.current_room->contains(variables.enemy_position.x, variables.enemy_position.y);
        },
        *variables,
        6
    );

    DTNode *if_enemy_not_in_room = new DTNode(
        [](ExposedVariables & variables) -> bool{
            // std::cout << "in the if_enemy_not_in_room" << std::endl;
            return !variables.current_room->contains(variables.enemy_position.x, variables.enemy_position.y);
        },
        *variables,
        7
    );

    root->AddChild(is_paused); // leaf 5

    root->AddChild(is_not_paused);

        is_not_paused->AddChild(within_range);
            within_range->AddChild(if_path_empty); 
                if_path_empty->AddChild(if_global_path_empty); // leaf 0
                if_path_empty->AddChild(if_not_global_path_empty); // leaf 1

            within_range->AddChild(if_not_path_empty); // leaf 2

        is_not_paused->AddChild(not_within_range);
            not_within_range->AddChild(if_path_not_set); // leaf 3
            not_within_range->AddChild(if_path_set); 
                if_path_set->AddChild(if_enemy_in_room); // leaf 6
                if_path_set->AddChild(if_enemy_not_in_room); // leaf 7

    return root;

}
#endif


