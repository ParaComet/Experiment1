#include "res.h"
#include <iostream>

using namespace TASK1;

ResNet::ResNet() {
    std::cout << "RES constructor called" << std::endl;
}

ResNet::~ResNet() {
    std::cout << "RES destructor called" << std::endl;
}


void ResNet::addNode(const Name& name) {
    // TODO: add node to resNet 
    if (nameToIndex_.find(name) == nameToIndex_.end()) {
        nodes_.push_back(name);
        nameToIndex_[name] = nodes_.size() - 1;
    }
}   

void ResNet::addEdge(const Name& src, const Name& dst, Value value) {
    // 确保两个节点都存在
    addNode(src);
    addNode(dst);

    // 获取源节点的索引
    size_t srcIndex = nameToIndex_[src];

    // 检查边是否已经存在
    auto& edge = nodes_[srcIndex].edges;
    for(auto& e : edge) {
        if(e.first == dst) {
            e.second = value;
            return;
        }
    }
    // 边不存在，添加边
    edge.push_back(std::make_pair(dst, value));

}

void ResNet::printGraph() const{
    // 打印节点
    for(const auto& node : nodes_) {
        std::cout << "Node: " << node.name << std::endl;
        // 打印边
        for(const auto& edge : node.edges) {
            std::cout << "    " << edge.first << " " << edge.second << std::endl;
        }
        std::cout << std::endl;
    }
}


