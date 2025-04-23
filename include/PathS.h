#ifndef PATHS_H
#define PATHS_H

#include "res.h"
#include <iostream>

using namespace TASK1;

namespace TASK1 {
    class PathS {
    public:
        PathS();
        ~PathS();
        void searchPath(const ResNet& net);
        void dfsSearch(const ResNet& resNet, size_t currentIndex, 
                      std::vector<bool>& visited, 
                      std::vector<Name>& currentPath, 
                      int currntWeight);  
        void printPath(std::vector<Name> path);
    private:
        struct pathNode {
            Name name;
            std::vector<Name> pathToHere;
            int totalWeight;
            pathNode(Name n) : name(n), pathToHere(), totalWeight(0) {}
        };
        std::vector<pathNode> Node;
        std::unordered_map<Name, std::vector<pathNode>> graph;
    };
}


#endif // PATHS_H