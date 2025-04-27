#include "PathS.h"
#include "res.h"
#include <cstddef>
#include <iostream>

using namespace TASK1;

PathS::PathS(const ResNet& net) : resNet_(net) {
    //std::cout << "PathS constructor called" << std::endl;
}

PathS::~PathS() {
    //std::cout << "PathS destructor called" << std::endl;
}

void PathS::searchPath() {
    // 清空之前的结果
    Node_.clear();
    
    // 初始化起点
    for (const auto& node : resNet_.nodes_) {
        pathNode pn(node.name);
        Node_.push_back(pn);
    }

    // 对每个节点作为起点进行DFS搜索
    for (size_t i = 0; i < resNet_.nodes_.size(); ++i) {
        std::vector<bool> visited(resNet_.nodes_.size(), false);
        std::vector<Name> currentPath;
        Value currentWeight = 0;
        
        dfsSearch(resNet_, i, visited, currentPath, currentWeight, i);

    }
}

void PathS::dfsSearch(const ResNet& resNet, size_t currentIndex, 
                     std::vector<bool>& visited, 
                     std::vector<Name>& currentPath,
                     Value currentWeight, size_t& starIndex) {
    // 标记当前节点为已访问
    visited[currentIndex] = true;
    currentPath.push_back(resNet.nodes_[currentIndex].name);
    
    // 记录当前路径仅有节点大于二时为有效
    if (currentPath.size() > 1) {
        for(auto& node : Node_) {
            // 找到起点节点
            if (node.name == resNet.nodes_[starIndex].name) {
                node.paths.push_back({currentPath, currentWeight});
                break;
            }
        }
    }
    
    // 遍历所有邻接节点
    for (const auto& edge : resNet.nodes_[currentIndex].edges) {
        size_t nextIndex = resNet.nameToIndex_.at(edge.first);
        
        if (!visited[nextIndex]) {
            // 递归搜索
            dfsSearch(resNet, nextIndex, visited, currentPath, 
                     currentWeight + edge.second, starIndex);
        }
    }
    
    // 回溯
    visited[currentIndex] = false;
    currentPath.pop_back();
}

void PathS::printPath(const std::vector<Name>& path, Value weight, int type) {
    if (path.empty()) {
        std::cout << "No path found" << std::endl;
        return;
    }
    // 打印路径
    for (size_t i = 0; i < path.size(); ++i) {
        if (i != 0) {
            std::cout << " -> ";
        }
        std::cout << path[i];
    }
    // 打印路径权重
    if (type == 0) {
        std::cout << " (Total weight: " << weight << ")" << std::endl;

    } else if (type == 1) {
        std::cout << " -> ";
    }// type ==1 为打印环路
}

void PathS::printAllPaths() {
    // 打印所有路径
    for (const auto& node : Node_) {
        std::cout << "Paths starting from " << node.name << ":" << std::endl;
        if (node.paths.empty()) {
            std::cout << "No path found" << std::endl;
            std::cout << std::endl;
            continue;
        }
        for(const auto& path : node.paths) {
            printPath(path.first, path.second, 0); 
        }
        std::cout << std::endl;
    }
    return ;
}

void PathS::printPathto(const Name& from, const Name& to, Value weight, int type) {
    // 打印从path到to的路径
    bool found = false;
    for (const auto& node : Node_) {
        if (node.name == from) {
            for(const auto& path : node.paths) {
                if (path.first.back() == to) {
                    printPath(path.first, path.second, type);
                    if (type == 1) {
                        std::cout << from << " (Total weight: " << path.second+weight << ")" <<std::endl;
                    }
                    found = true;
                }
            }
            if (found) {
                return;
            }
            std::cout << "No path found" << std::endl;
            return;
        }
    }
    std::cout << "Node " << from << " not found" << std::endl;
    return;
}