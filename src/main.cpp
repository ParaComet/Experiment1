﻿#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>
#include "res.h"
#include "Console.h"
#include <fstream> // Required for file output
#include <chrono>   // Required for std::chrono
#include <iomanip>  // Required for std::put_time (though not strictly needed for epoch seconds)
#include <string> // Required for std::string manipulation

using namespace TASK1;

void initialize_json_files() {
    std::ofstream graph_file("../bin/graph_data.json");
    if (graph_file.is_open()) {
        graph_file << "{\n  \"nodes\": [],\n  \"edges\": []\n}";
        graph_file.close();
    } else {
        std::cerr << "Unable to open graph_data.json for writing." << std::endl;
    }

    std::ofstream signal_file("../bin/update_signal.json");
    if (signal_file.is_open()) {
        signal_file << "{\"timestamp\": 0}";
        signal_file.close();
    } else {
        std::cerr << "Unable to open update_signal.json for writing." << std::endl;
    }
}

int main() {
    initialize_json_files(); // Initialize JSON files at the start
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    ResNet graph;
    //int* num = new int(10);   //检测内存是否泄露
    //std::cout << "num: " << *num << std::endl;
    Console console(graph);
    console.start();


    //_CrtDumpMemoryLeaks();
/*  Heap minHeap;
    
    python -m http.server 8000

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
/*     
     graph.addNode('A');
     graph.addNode('B');
     graph.addNode('C');    
     graph.addNode('D');
     
     graph.addEdge('A', 'B', 2);
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
            if(n!= path.back()) {
                std::cout << n << "->";
            }
            else {
                std::cout << n << std::endl;
            }
         }
     }
*/

/*   auto start = std::chrono::high_resolution_clock::now();
     if(graph.CircleCheck('A', 'D')) {
         std::cout << "Circle found" << std::endl;
     };
     auto end = std::chrono::high_resolution_clock::now();
     auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
     std::cout << "Time used: " << duration.count() << " microseconds" << std::endl;
*/
/*     std::cout << std::endl;

     PathS pathFinder(graph);
     pathFinder.searchPath();
     pathFinder.printAllPaths();
*/    
    return 0;
}
