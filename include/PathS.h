#ifndef PATHS_H
#define PATHS_H

#include "res.h"
#include <cstddef>
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
                      Value currntWeight, size_t& starIndex);
        void printAllPaths();  
        void printPath(const std::vector<Name>& path, Value weight, int type);
        void printPathto(const Name& from, const Name& to, Value weight, int type);
        struct pathNode {
            Name name;
            std::vector<std::pair<std::vector<Name>, Value>> paths;

            pathNode(const Name& name) : name(name) {}
        };
        //存储当前图的节点信息
        const ResNet& resNet_;
        // 存储所有起始节点，pathNode为当前起点的所有可达路径
        std::vector<pathNode> Node_;
        // 存储当前搜索路径的图信息
        std::unordered_map<Name, std::vector<pathNode>> graph;
    };
}


#endif // PATHS_H