#ifndef DECISIONTREENODE_H
#define DECISIONTREENODE_H

#include <string>
#include <any>
#include <unordered_map>
#include <functional>
#include "ExposedVariables.h"

typedef std::function<bool(ExposedVariables &)> bool_function;

class DTNode
{
public:

    bool isLeaf;
    int leaf_id;
    u_int64_t last_call = 0;

    // Constructor
    DTNode(bool_function eval_function, ExposedVariables & variables, int leaf_id){

        this->eval_function = std::bind(eval_function, std::ref( variables ));

        this->children = std::vector<DTNode*>(); 
        this->leaf_id = leaf_id;
        if(leaf_id >= 0){
            this->isLeaf = true;
        }
        else{
            this->isLeaf = false;
        }
    }

    void AddChild(DTNode* child){
        children.push_back(child);
    }

    int evaluate(u_int64_t call_time){

        last_call = call_time;

        if(isLeaf){
            return leaf_id;
        }

        for (auto child : children){
            if (child->eval_function()){
                return child->evaluate(call_time);
            }

        }

        return -1;
    }

    // Destructor
    ~DTNode(){
        for (auto child : children){
            delete child;
        }

        // delete variables;        
    }

private:

    // TreeVariables & variables;

    // the boolean function to evaluate
    std::function<bool()> eval_function;

    std::vector <DTNode*> children;

};

#endif // DECISIONTREENODE_H