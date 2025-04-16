#include <iostream>
#include "res.h"
#include "Dijk.h"

using namespace TASK1;

void GetData(ResNet &res) {
    Name from, to;
    Value val;
    std::cout << "Enter numbers (Ctrl+Z to end input): ";
    while (std::cin >> from >> to >> val) {
        res.addEdge(from, to, val);
    }
}

int main() {
    ResNet graph;
/*  Heap minHeap;
    
    minHeap.insert('A', 5);
    minHeap.insert('B', 3);
    minHeap.insert('C', 7);
    minHeap.insert('D', 1);
    
    minHeap.decreaseKey('C', 2);
    
    while (!minHeap.isEmpty()) {
        std::cout << minHeap.extractMin() << std::endl;
    }
*/
   // 建立邻接表
    GetData(graph);
    
    // 打印邻接表
    std::cout << "Linked list contents: " << std::endl;
    graph.printGraph();

    return 0;
}
