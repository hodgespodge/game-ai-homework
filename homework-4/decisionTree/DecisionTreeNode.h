#ifndef DECISIONTREENODE_H
#define DECISIONTREENODE_H

#include <string>
#include <any>
#include <unordered_map>
#include <functional>

class DTNode
{
public:

    bool isLeaf;
    int leaf_id;

    // Constructor
    DTNode(std::function<bool(std::unordered_map<std::string, std::any>&)> bool_function, std::unordered_map<std::string, std::any>* variables, int leaf_id){
        this->bool_function = bool_function;
        this->variables = variables;
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

    int evaluate(){

        if(isLeaf){
            return leaf_id;
        }

        int index = 0;
        int num_children = children.size();

        for (auto child : children){
            if ( index == num_children ||  child->bool_function(*variables)){
                return child->evaluate();
            }

            index++;
        }

        return -1;
    }

    // Destructor
    ~DTNode(){
        for (auto child : children){
            delete child;
        }

        delete variables;        
    }

private:

    // unordered map of variable_names to untyped values
    std::unordered_map<std::string, std::any>* variables;

    // the boolean function to evaluate
    std::function<bool(std::unordered_map<std::string, std::any>&)> bool_function;

    std::vector <DTNode*> children;

};

#endif // DECISIONTREENODE_H