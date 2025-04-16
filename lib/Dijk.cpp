#include "Dijk.h"
#include <iostream>
using namespace TASK1;

Heap::Heap() {
    std::cout << "Heap created" << std::endl;
}

Heap::~Heap() {
    std::cout << "Heap destroyed" << std::endl;
}

//判断是否为空
bool Heap::isEmpty() const {
    return heap_.empty();
}

//上浮操作
void Heap::heapifyUp(int index) {
    //继承式更新避免内存开销过大
    while (index > 0) {
        int parentIndex = getParentIndex(index);
        
        if(heap_[index] < heap_[parentIndex]) {

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
    while(index < size) {

        //获取左右子节点的索引
        int leftChild = getLeftChildIndex(index);
        int rightChild = getRightChildIndex(index);
        int smallest = index;

        //找出左右子节点中最小的节点
        if(leftChild < size && heap_[leftChild] < heap_[smallest]) {
            smallest = leftChild;
        }

        if(rightChild < size && heap_[rightChild] < heap_[smallest]) {
            smallest = rightChild;
        }

        if(smallest != index) {
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

int Heap::getLeftChildIndex(int parentIndex) const{
    return 2 * parentIndex + 1;
}

int Heap::getRightChildIndex(int parentIndex) const{
    return 2 * parentIndex + 2;
}

int Heap::getParentIndex(int childIndex) const{
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

    heap_.pop_back();

    if(!isEmpty()) {
        //对堆顶元素进行下沉操作
        heapifyDown(0);
    }

    return minName;
}

void Heap::decreaseKey(const Name& name, const Value& dist) {
    if(nodeToIndex_.find(name) == nodeToIndex_.end()) {
        return;
    }
    
    int index = nodeToIndex_[name];

    //更新节点的距离
    if(heap_[index].dist > dist) {
        heap_[index].dist = dist;
        //对节点进行上浮操作
        heapifyUp(index);
    }
}


