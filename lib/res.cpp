#include "res.h"
#include <cstddef>
#include <iostream>
#include <queue>
#include "PathS.h"
#include "Color.h"
#include <algorithm>

using namespace TASK1;

ResNet::ResNet() {
    //std::cout << "RES constructor called" << std::endl;
}

ResNet::~ResNet() {
    //std::cout << "RES destructor called" << std::endl;
}


int ResNet::addNode(const Name& name) {
    // TODO: add node to resNet 
    int index = findNode(name);//查找节点在图中的索引
    if (index == -1) {
        nodes_.push_back(name);
        errnodes_.push_back(name);
        nameToIndex_[name] = nodes_.size() - 1;
        return nodes_.size() - 1;
    }
    return index;
}   

int ResNet::findNode(const Name& name) const {
    //查找节点是否在图中
    if (nameToIndex_.find(name) == nameToIndex_.end()) {
        return -1;
    }
    return nameToIndex_.at(name);
}

int ResNet::CircleCheck(const Name& src, const Name& dst) {
    // TODO: check if there is a circle in the resNet
    if (src == dst) {
        return 1;
    }
    
    bool flag = false;

    if (findNode(dst) == -1) {
        addNode(dst);
        flag = true;
    }
    if(findNode(src) == -1) {
        addNode(src);
        flag = true;
    }
    if (flag) {
        std::cout << Color::colorize("No circle, node not found",Color::YELLOW,Color::BOLD) << std::endl;
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
    std::cout << Color::colorize("No circle found", Color::YELLOW, Color::BOLD) << std::endl;
    return 0;
}

int ResNet::addEdge(const Name& src, const Name& dst, Value value) {
    //检查目标节点到源节点是否存在路径
    if(CircleCheck(dst,src)) {
        std::cout << Color::colorize("Cannot add edge, there is a circle in the resNet",Color::RED,Color::BOLD) << std::endl;
        size_t srcIndex = nameToIndex_[src];
        size_t dstIndex = nameToIndex_[dst];
        //检查边是否已经存在
        for (auto &edge : errnodes_[srcIndex].edges) {
            if (edge.first == dst) {
                edge.second = value;
                return 1;
            }
        }
        //边不存在，添加边
        errnodes_[srcIndex].edges.push_back(std::make_pair(dst, value));
        errnodes_[srcIndex].num_edges++;
        return 1;
    }

    // 获取源节点的索引
    size_t srcIndex = nameToIndex_[src];


    // 检查边是否已经存在
    auto& edge = nodes_[srcIndex].edges;
    for (auto& e : edge) {
        if (e.first == dst) {
            e.second = value;
            return 2;
        }
    }
    // 边不存在，添加边
    edge.push_back(std::make_pair(dst, value));
    nodes_[srcIndex].num_edges++;
    return 0;
}

void ResNet::printGraph() const {
    using namespace TASK1;
    if (nodes_.empty()) {
        std::cout << Color::colorize("No graph exists", Color::RED) << std::endl;
        return;
    }

    // 打印标题
    std::cout << Color::colorize("Graph Structure:", Color::GREEN, Color::BOLD) << std::endl;
    
    // 打印节点和边
    for (const auto& node : nodes_) {
        // 打印节点名称
        std::cout << Color::colorize("Node: ", Color::CYAN) 
                  << Color::colorize(std::string(1, node.name), Color::YELLOW, Color::BOLD) 
                  << std::endl;
        
        // 打印边
        if (node.edges.empty()) {
            std::cout << Color::colorize("    No edges", Color::MAGENTA) << std::endl;
        } else {
            for (const auto& edge : node.edges) {
                std::cout << "    " 
                          << Color::colorize(std::string(1, edge.first), Color::BLUE)
                          << " " 
                          << Color::colorize(std::to_string(edge.second), Color::MAGENTA)
                          << std::endl;
            }
        }
        std::cout << std::endl;
    }
}

const std::vector<std::pair<Name, Value>>& ResNet::getEdges(const Name& node) const {
    // 获取一个节点的所有邻接边
    auto it = nameToIndex_.find(node);
    if (it != nameToIndex_.end()) {
        return nodes_[it->second].edges;
    }
    static std::vector<std::pair<Name, Value>> empty; // 返回空引用
    return empty;
}

int ResNet::removeEdge(const Name& src, const Name& dst) {
    // 检查源节点是否存在
    if (nameToIndex_.find(src) == nameToIndex_.end()) {
        return 1;
    }
    if (nameToIndex_.find(dst) == nameToIndex_.end()) {
        return 1;
    }

    size_t srcIndex = nameToIndex_[src];
    size_t dstIndex = nameToIndex_[dst];

    // 先检查并删除非法边
    for (auto it = errnodes_[srcIndex].edges.begin(); it != errnodes_[srcIndex].edges.end(); ) {
        if (it->first == dst) {
            it = errnodes_[srcIndex].edges.erase(it);
            errnodes_[srcIndex].num_edges--;
            return 2;
        } else {
            ++it;
        }
    }

    // 检查并删除正常边
    bool edgeFound = false;
    for (auto it = nodes_[srcIndex].edges.begin(); it != nodes_[srcIndex].edges.end(); ) {
        if (it->first == dst) {
            it = nodes_[srcIndex].edges.erase(it);
            nodes_[srcIndex].num_edges--;
            edgeFound = true;
            break;
        } else {
            ++it;
        }
    }

    if (!edgeFound) {
        return 1;
    }

    // 处理可能因此变为合法的非法边
    std::vector<std::pair<Name, Name>> edgesToLegalize;
    
    // 先收集需要合法化的边，避免在遍历时修改容器
    for (const auto& ernode : errnodes_) {
        for (const auto& edge : ernode.edges) {
            if (!CircleCheck(ernode.name, edge.first)) {
                edgesToLegalize.push_back(std::make_pair(ernode.name, edge.first));
            }
        }
    }

    // 处理收集到的边（C++11 中替换结构化绑定）
    for (const auto& edge_pair : edgesToLegalize) {
        const Name& from = edge_pair.first;
        const Name& to = edge_pair.second;

        auto it = std::find_if(errnodes_[nameToIndex_[from]].edges.begin(),
                              errnodes_[nameToIndex_[from]].edges.end(),
                              [&to](const std::pair<Name, Value>& e) { return e.first == to; });
        if (it != errnodes_[nameToIndex_[from]].edges.end()) {
            Value val = it->second;
            if (!addEdge(from, to, val)) {
                errnodes_[nameToIndex_[from]].edges.erase(it);
                errnodes_[nameToIndex_[from]].num_edges--;
            }
        }
    }

    return 0;
}



int ResNet::removeNode(const Name& name) {
    //检查节点是否存在
    if (nameToIndex_.find(name) == nameToIndex_.end()) {
        return 1;
    }
    //获取节点的索引
    size_t index = nameToIndex_[name];
    //删除节点
    nodes_.erase(nodes_.begin() + index);
    errnodes_.erase(errnodes_.begin() + index);
    //更新nameToIndex_
    for (auto it = nameToIndex_.begin(); it != nameToIndex_.end(); ++it) {
        if (it->second > index) {
            it->second--;
        }
        //删除节点的边
        for (auto edgeIt = nodes_[it->second].edges.begin(); edgeIt != nodes_[it->second].edges.end(); ) {
            if (edgeIt->first == name) {
                edgeIt = nodes_[it->second].edges.erase(edgeIt);
                nodes_[it->second].num_edges--;
            } else {
                ++edgeIt;
            }
        }
        for (auto edgeIt = errnodes_[it->second].edges.begin(); edgeIt != errnodes_[it->second].edges.end(); ) {
            if (edgeIt->first == name) {
                edgeIt = errnodes_[it->second].edges.erase(edgeIt);
                errnodes_[it->second].num_edges--;
            } else {
                ++edgeIt;
            }
        }
    }
    nameToIndex_.erase(name);
    return 0;
}

int ResNet::printErrNodes() const {
    using namespace TASK1;
    
    // 打印节点
    if (errnodes_.empty()) {
        std::cout << Color::colorize("No graph exists", Color::RED) << std::endl;
        return 1;
    }

    // 打印标题
    std::cout << Color::colorize("Error edges:", Color::RED, Color::BOLD) << std::endl;
    
    // 打印错误节点和边
    for (const auto& node : errnodes_) {
        // 打印节点名称
        std::cout << Color::colorize("Node: ", Color::CYAN) 
                  << Color::colorize(std::string(1, node.name), Color::YELLOW, Color::BOLD) 
                  << std::endl;
        
        // 打印错误边
        if (node.edges.empty()) {
            std::cout << Color::colorize("    No error edges", Color::MAGENTA) << std::endl;
        } else {
            for (const auto& edge : node.edges) {
                std::cout << "    " 
                          << Color::colorize(std::string(1, edge.first), Color::RED)
                          << " " 
                          << Color::colorize(std::to_string(edge.second), Color::MAGENTA)
                          << std::endl;
            }
        }
        std::cout << std::endl;
    }
    return 0;
}
