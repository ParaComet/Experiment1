#include "Dijk.h"
#include <iostream>
#include <algorithm>
using namespace TASK1;

// Dijkstra算法实现
Dijkstra::Dijkstra(const ResNet& graph) : graph_(graph) {
    distances_.clear();
    predecessors_.clear();
    //std::cout << "Dijkstra created" << std::endl;
}

Dijkstra::~Dijkstra() {
    //std::cout << "Dijkstra destroyed" << std::endl<< std::endl;
}


void Dijkstra::shortestPath(const Name& start) {
    // 初始化距离和前驱
    distances_.clear();
    predecessors_.clear();
    
    for (const auto& node : graph_.nodes_) {
        distances_[node.name] = INF;
    }
    distances_[start] = 0;
    
    // 初始化优先队列
    Heap minHeap;

    // 将所有节点加入优先队列
    for (const auto& node : graph_.nodes_) {
        minHeap.insert(node.name, distances_[node.name]);
    }
    
    // 主循环
    while (!minHeap.isEmpty()) {
        Name u = minHeap.extractMin();
        
        // 如果当前节点的距离是INF，说明剩余节点都不可达
        if (distances_[u] == INF) break;
        
        // 遍历所有邻接节点
        for (const auto& edge : graph_.getEdges(u)) {
            Name v = edge.first;
            Value weight = edge.second;
            
            Value alt = distances_[u] + weight;
            if (alt < distances_[v]) {
                distances_[v] = alt;
                predecessors_[v] = u;
                minHeap.decreaseKey(v, alt);
            }
        }
    }
}

Value Dijkstra::getDistance(const Name& node) const {
    // 获取节点的距离
    auto it = distances_.find(node);
    if (it != distances_.end()) {
        return it->second;
    }
    return INF;
}

std::vector<Name> Dijkstra::getPath(const Name& node) const {
    std::vector<Name> path;
    if (distances_.find(node) == distances_.end() || distances_.at(node) == INF) {
        return path; // 返回空路径
    }
    
    Name current = node;
    while (predecessors_.find(current) != predecessors_.end()) {
        path.push_back(current);
        current = predecessors_.at(current);
    }
    path.push_back(current); // 添加起点
    
    std::reverse(path.begin(), path.end());
    return path;
}

Heap::Heap() {
    //std::cout << "Heap created" << std::endl;
}

Heap::~Heap() {
    //std::cout << "Heap destroyed" << std::endl;
}


//判断是否为空
bool Heap::isEmpty() const {
    return heap_.empty();
}

//上浮操作
void Heap::heapifyUp(int index) {
    //迭代递归避免内存开销过大
    while (index > 0) {
        int parentIndex = getParentIndex(index);
        
        if (heap_[index] < heap_[parentIndex]) {

            //交换子节点和父节点
            HeapNode temp = heap_[index];
            heap_[index] = heap_[parentIndex];
            heap_[parentIndex] = temp;

            //更新索引映射
            nodeToIndex_[heap_[index].name] = index;
            nodeToIndex_[heap_[parentIndex].name] = parentIndex;
            
            index = parentIndex;
        }
        else {
            break;
        }
    }
}    

//下沉操作
void Heap::heapifyDown(int index) {
    int size = heap_.size();
    while (index < size) {

        //获取左右子节点的索引
        int leftChild = getLeftChildIndex(index);
        int rightChild = getRightChildIndex(index);
        int smallest = index;

        //找出左右子节点中最小的节点
        if (leftChild < size && heap_[leftChild] < heap_[smallest]) {
            smallest = leftChild;
        }

        if (rightChild < size && heap_[rightChild] < heap_[smallest]) {
            smallest = rightChild;
        }

        if (smallest != index) {
            //交换子节点和父节点
            HeapNode temp = heap_[index];
            heap_[index] = heap_[smallest];
            heap_[smallest] = temp;

            //更新索引映射
            nodeToIndex_[heap_[index].name] = index;
            nodeToIndex_[heap_[smallest].name] = smallest;

            index = smallest;
        }
        else {
            break;
        }
    }
}

//插入操作
void Heap::insert(const Name& name, const Value& dist) {
   // std::cout << "Insert " << name << " with distance " << dist << std::endl;
    heap_.push_back({name,dist});
    nodeToIndex_[name] = heap_.size() - 1;
    //对新插入的节点进行上浮操作
    heapifyUp(heap_.size() - 1);
}

int Heap::getLeftChildIndex(int parentIndex) const {
    return 2 * parentIndex + 1;
}

int Heap::getRightChildIndex(int parentIndex) const {
    return 2 * parentIndex + 2;
}

int Heap::getParentIndex(int childIndex) const {
    return (childIndex - 1) / 2;
}

//获取堆顶元素
Name Heap::extractMin(){
    if (isEmpty()) {
        throw std::runtime_error("Heap is empty");
    }

    //获取堆顶元素
    Name minName = heap_[0].name;
    nodeToIndex_.erase(minName);

    if (heap_.size() > 1) {
        //将最后一个元素放到堆顶
        heap_[0] = heap_[heap_.size() - 1];
        nodeToIndex_[heap_[0].name] = 0;
    }
    //交换堆顶元素和最后一个元素

    //删除最后一个元素
    heap_.pop_back();

    if (!isEmpty()) {
        //对堆顶元素进行下沉操作
        heapifyDown(0);
    }

    return minName;
}

void Heap::decreaseKey(const Name& name, const Value& dist) {
    if (nodeToIndex_.find(name) == nodeToIndex_.end()) {
        return;
    }
    
    int index = nodeToIndex_[name];

    //更新节点的距离
    if (heap_[index].dist > dist) {
        heap_[index].dist = dist;
        //对节点进行上浮操作
        heapifyUp(index);
    }
}


