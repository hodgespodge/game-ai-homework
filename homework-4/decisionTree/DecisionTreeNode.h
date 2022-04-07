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
    

    // Constructor
    DTNode(std::function<bool(std::unordered_map<std::string, std::any>&)> bool_function, std::unordered_map<std::string, std::any>* variables, bool isLeaf){
        this->bool_function = bool_function;
        this->variables = variables;
        this->isLeaf = isLeaf;
        this->children = std::vector<DTNode*>();
        
    }

    void AddChild(DTNode* child){
        children.push_back(child);
    }

    // void 

    // evaluate()

    // Destructor
    ~DTNode();

private:

    // unordered map of variable_names to untyped values
    std::unordered_map<std::string, std::any>* variables;

    // the boolean function to evaluate
    std::function<bool(std::unordered_map<std::string, std::any>&)> bool_function;
    // std::unordered_map<std::string, std::any>& evaluation_function;
    
    std::vector <DTNode*> children;

};

#endif // DECISIONTREENODE_H