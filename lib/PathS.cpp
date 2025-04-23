#include "PathS.h"
#include "res.h"
#include <iostream>

using namespace TASK1;

PathS::PathS() {
    std::cout << "PathS constructor called" << std::endl;
}

PathS::~PathS() {
    std::cout << "PathS destructor called" << std::endl;
}

void PathS::searchPath(const ResNet& resNet) {
    // 清空之前的结果
    Node.clear();
    
    // 初始化所有节点
    for (const auto& node : resNet.nodes_) {
        pathNode pn(node.name);
        Node.push_back(pn);
    }
    
    // 对每个节点作为起点进行DFS搜索
    for (size_t i = 0; i < resNet.nodes_.size(); ++i) {
        std::vector<bool> visited(resNet.nodes_.size(), false);
        std::vector<Name> currentPath;
        int currentWeight = 0;
        
        dfsSearch(resNet, i, visited, currentPath, currentWeight);
    }
}

void PathS::dfsSearch(const ResNet& resNet, size_t currentIndex, 
                     std::vector<bool>& visited, 
                     std::vector<Name>& currentPath,
                     int currentWeight) {
    // 标记当前节点为已访问
    visited[currentIndex] = true;
    currentPath.push_back(resNet.nodes_[currentIndex].name);
    
    // 记录当前路径
    pathNode pn(resNet.nodes_[currentIndex].name);
    pn.pathToHere = currentPath;
    pn.totalWeight = currentWeight;
    Node.push_back(pn);
    
    // 遍历所有邻接节点
    for (const auto& edge : resNet.nodes_[currentIndex].edges) {
        size_t nextIndex = resNet.nameToIndex_.at(edge.first);
        
        if (!visited[nextIndex]) {
            // 递归搜索
            dfsSearch(resNet, nextIndex, visited, currentPath, 
                     currentWeight + edge.second);
        }
    }
    
    // 回溯
    visited[currentIndex] = false;
    currentPath.pop_back();
}

void PathS::printPath(std::vector<Name> path) {
    if (path.empty()) {
        std::cout << "No path found" << std::endl;
        return;
    }
    
    for (size_t i = 0; i < path.size(); ++i) {
        if (i != 0) std::cout << " -> ";
        std::cout << path[i];
    }
    std::cout << std::endl;
}
