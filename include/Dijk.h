#ifndef DIJK_H
#define DIJK_H

#include "res.h"
#include <unordered_map>
#include <vector>

using namespace TASK1;

namespace TASK1 {
    void dijKstra(const ResNet& ResNet);
    
    

    class Heap{
    public:
        Heap();
        ~Heap();
        void insert(const Name& node, const Value& dist);
        Name extractMin();
        bool isEmpty() const;
        void decreaseKey(const Name& node, const Value& dist);

    private:
    // private member variables
        int size;
        
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