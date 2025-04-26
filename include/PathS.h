#ifndef PATHS_H
#define PATHS_H

#include "res.h"
#include <iostream>

using namespace TASK1;

namespace TASK1 {
    class PathS {
    public:
        PathS(const ResNet& net);
        ~PathS();
        void searchPath();
        void dfsSearch(const ResNet& resNet, size_t currentIndex, 
                      std::vector<bool>& visited, 
                      std::vector<Name>& currentPath, 
                      int currntWeight);
        int printNodepath(const Name& start);  
    private:
        //存储当前图的节点信息
        const ResNet& resNet_;
        struct pathNode {
            Name name;
            std::vector<Name> pathToHere;
            int totalWeight;
            pathNode(Name n) : name(n), pathToHere(), totalWeight(0) {}
        };
        std::vector<pathNode> Node;
        std::unordered_map<Name, std::vector<pathNode>> graph;
        void printPath(std::vector<pathNode> path);
    };
}


#endif // PATHS_H