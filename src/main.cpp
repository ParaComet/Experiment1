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
/*
// 建立邻接表
    GetData(graph);
    
    // 打印邻接表
    std::cout << "Linked list contents: " << std::endl;
    graph.printGraph();


*/   
     // 1. 创建图
     
     graph.addNode('A');
     graph.addNode('B');
     graph.addNode('C');    
     graph.addNode('D');
     
     graph.addEdge('A', 'B', 4);
     graph.addEdge('A', 'C', 2);
     graph.addEdge('B', 'D', 5);
     graph.addEdge('C', 'B', 1);
     graph.addEdge('C', 'D', 8);
     
     // 2. 运行Dijkstra算法
     Dijkstra dijkstra(graph);
     dijkstra.shortestPath('A');
     
     // 3. 输出结果
     for (const auto& node : graph.nodes_) {
         std::cout << "Distance to " << node.name << ": " 
                   << dijkstra.getDistance(node.name) << std::endl;
         
         std::cout << "Path: ";
         auto path = dijkstra.getPath(node.name);
         for (const auto& n : path) {
             std::cout << n << " ";
         }
         std::cout << std::endl;
     }
    return 0;
}
