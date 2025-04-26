#ifndef DIJK_H
#define DIJK_H

#include "res.h"
#include <unordered_map>
#include <vector>
#include <limits>

using namespace TASK1;

namespace TASK1 {
    
    //定义无穷大常数
    const Value INF = std::numeric_limits<Value>::max();
    
    //Dijkstra算法
    class Dijkstra {
    public:
        Dijkstra(const ResNet& graph);
        
        void shortestPath(const Name& start);
        Value getDistance(const Name& Djnode) const;
        std::vector<Name> getPath(const Name &Djnode) const;
        int printPath(const Name& start, const Name& end);

    private:
        const ResNet& graph_;
        std::unordered_map<Name,Value> distances_;
        std::unordered_map<Name,Name> predecessors_;
    };    
    

    //堆的实现
    class Heap{
    public:
        Heap();
        ~Heap();
        void insert(const Name& Djnode, const Value& dist);
        Name extractMin();
        bool isEmpty() const;
        void decreaseKey(const Name& Djnode, const Value& dist);

    private:
    // private member variables
        int size_;
        
        //上浮操作
        void heapifyUp(int index);
        
        //下沉操作
        void heapifyDown(int index);
        
        //得到左节点的索引
        int getLeftChildIndex(int parentIndex) const;

        //得到右节点的索引
        int getRightChildIndex(int parentIndex) const;

        //得到父节点的索引
        int getParentIndex(int childIndex) const;

        //堆节点定义
        struct HeapNode {
            Name name;
            Value dist;
            bool operator<(const HeapNode& other) const {
                return dist < other.dist;
            }
        };

        std::vector<HeapNode> heap_;
        //节点到索引的映射
        std::unordered_map<Name, int> nodeToIndex_;
    };

}
#endif // DIJK_H