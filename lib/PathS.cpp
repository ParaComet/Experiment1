#include "PathS.h"
#include "res.h"
#include <iostream>

using namespace TASK1;

PathS::PathS(const ResNet& net) : resNet_(net) {
    std::cout << "PathS constructor called" << std::endl;
}

PathS::~PathS() {
    std::cout << "PathS destructor called" << std::endl;
}

void PathS::searchPath() {
    // 清空之前的结果
    Node.clear();
    
    // 对每个节点作为起点进行DFS搜索
    for (size_t i = 0; i < resNet_.nodes_.size(); ++i) {
        std::vector<bool> visited(resNet_.nodes_.size(), false);
        std::vector<Name> currentPath;
        int currentWeight = 0;
        
        dfsSearch(resNet_, i, visited, currentPath, currentWeight);
        // 将当前起点的搜索结果加入到map中，以便后续查找
        // 注意：这里的Node是局部变量，在搜索结束后会被清空
        graph[resNet_.nodes_[i].name] = Node;
        // 打印当前搜索结果
        printPath(Node);
        // 清空当前搜索结果
        Node.clear();
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

void PathS::printPath(std::vector<pathNode> path) {
    // 打印路径
    // path[0]为起点，path[i].name为终点，path[i].pathToHere为到终点的路径，path[i].totalWeight为当前路径距离
    Name start = path[0].name;
    
    // 路径为空
    if (path.size() == 1) {
        std :: cout << "There is no path from " << start << " to any other node" << std :: endl;
        return;
    }

    for (auto& node : path) {

        if (node.name == start) {
            continue;
        }

        std::cout << "From " << start << " to " << node.name << " : " << std::endl;
        for (auto& name : node.pathToHere) {

            if (name == node.pathToHere.back()) {
                std::cout << name << std::endl;
            }
            else {
                std::cout << name << " -> ";
            }
        }
        std::cout << "Total weight: " << node.totalWeight << std::endl;
    }
}

int PathS::printNodepath(const Name& start) {
    return 0;
}
