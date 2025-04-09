#include <iostream>
#include "res.h"

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
    
    // 建立邻接表
    GetData(graph);
    
    // 打印邻接表
    std::cout << "Linked list contents: " << std::endl;
    graph.printGraph();

    return 0;
}
