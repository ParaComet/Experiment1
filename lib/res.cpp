#include "res.h"
#include <cstddef>
#include <iostream>
#include <queue>

using namespace TASK1;

ResNet::ResNet() {
    std::cout << "RES constructor called" << std::endl;
}

ResNet::~ResNet() {
    std::cout << "RES destructor called" << std::endl;
}


int ResNet::addNode(const Name& name) {
    // TODO: add node to resNet 
    if (nameToIndex_.find(name) == nameToIndex_.end()) {
        nodes_.push_back(name);
        nameToIndex_[name] = nodes_.size() - 1;
        return nodes_.size() - 1;
    }
    return 0;
}   

int ResNet::CircleCheck(const Name& src, const Name& dst) {
    // TODO: check if there is a circle in the resNet
    if(addNode(src) || addNode(dst)) {
        return 0;
    }
    
    size_t srcIndex = nameToIndex_[src];
    size_t dstIndex = nameToIndex_[dst];
    //检查是否访问过该节点
    std::vector<bool> visited(nodes_.size(), false);
    std::queue<size_t> q;
    q.push(srcIndex);
    visited[srcIndex] = true;

    while(!q.empty()) {
        size_t curIndex = q.front();
        q.pop();
        //如果找到节点，返回1
        if (curIndex == dstIndex) {
            return 1;
        }
        //遍历当前节点的边
        for (const auto& edge : nodes_[curIndex].edges) {
            size_t nextIndex = nameToIndex_[edge.first];
            if (!visited[nextIndex]) {
                q.push(nextIndex);
                visited[nextIndex] = true;
            }
        }
    }
    std::cout << "No circle found" << std::endl;
    return 0;
}

void ResNet::addEdge(const Name& src, const Name& dst, Value value) {
    //检查目标节点到源节点是否存在路径
    if(CircleCheck(dst,src)) {
        std::cout << "Cannot add edge, there is a circle in the resNet" << std::endl;
        return;
    }

    // 获取源节点的索引
    size_t srcIndex = nameToIndex_[src];


    // 检查边是否已经存在
    auto& edge = nodes_[srcIndex].edges;
    for (auto& e : edge) {
        if (e.first == dst) {
            e.second = value;
            return;
        }
    }
    // 边不存在，添加边
    edge.push_back(std::make_pair(dst, value));
    nodes_[srcIndex].num_edges++;
    return;
}

void ResNet::printGraph() const {
    // 打印节点
    for (const auto& node : nodes_) {
        std::cout << "Node: " << node.name << std::endl;
        // 打印边
        for (const auto& edge : node.edges) {
            std::cout << "    " << edge.first << " " << edge.second << std::endl;
        }
        std::cout << std::endl;
    }
}

const std::vector<std::pair<Name, Value>>& ResNet::getEdges(const Name& node) const {
    auto it = nameToIndex_.find(node);
    if (it != nameToIndex_.end()) {
        return nodes_[it->second].edges;
    }
    static std::vector<std::pair<Name, Value>> empty; // 返回空引用
    return empty;
}



