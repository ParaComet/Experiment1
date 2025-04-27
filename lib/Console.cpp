#include "Console.h"
#include "PathS.h"
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include "Color.h"

using namespace TASK1;

Console::Console(ResNet& graph) : graph_(graph) {
    //std::cout << "Graph processing console initialized.\n";
}

void Console::start() {
    std::cout << "Graph Processing Console (type 'help' for commands)" << std::endl;
    
    while (true) {
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty()) continue;
        
        // 转换为小写以便不区分大小写
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);
        
        if (input == "exit" || input == "quit") {
            break;
        }
        
        processCommand(input);
    }
}

void Console::printHelp() const {
    std::cout << "Available commands:\n"
              << "  help - Show this help message\n"
              << "  addnode - Add a node to the graph\n"
              << "  addedge - Add multiple edges (enter 'done' when finished)\n"
              << "  shortest - Find shortest path between two nodes\n"
              << "  allpaths - Find all paths in the graph\n"
              << "  dijk - Find all shortest paths in the graph\n"
              << "  print - Print the graph structure\n"
              << "  dlnode - Delete a node from the graph\n"
              << "  dledge - Delete an edge from the graph\n"
              << "  printpath - Print paths from start to end in the graph\n"
              << "  printcicle - Print all cycles in the graph\n"
              << "  exit/quit - Exit the program\n";
}


void Console::processCommand(const std::string& cmd) {
    if (cmd == "help") {
        printHelp();
    } else if (cmd == "addnode") {
        addNode();
    } else if (cmd == "addedge") {
        addEdge();
    } else if (cmd == "shortest") {
        findShortestPath();
    } else if (cmd == "dijk") {
        findAllShortestPaths();
    } else if (cmd == "allpaths") {
        findAllPaths();
    } else if (cmd == "print") {
        printGraph();
    } else if (cmd == "dlnode") {
        deleteNode();
    } else if (cmd == "dledge") {
        deleteEdge();
    } else if (cmd == "printpath") {
        printPath();
    } else if (cmd == "printcicle") {
        printCicle();
    } else {
        std::cout << "Unknown command. Type 'help' for available commands.\n";
    }
}

void Console::addNode() {
    std::cout << "Enter node name (single character): ";
    Name name;
    std::cin >> name;
    std::cin.ignore();  // 清除输入缓冲区
    
    name = toupper(name);

    if (graph_.addNode(name) == -1) {
        std::cout << "Node " << name << " already exists.\n";
    } else {
        std::cout << "Node " << name << " added successfully.\n";
    }
}

void Console::addEdge() {
    std::cout << "Enter edges (source destination weight), one per line. Enter 'done' when finished:\n";
    
    while (true) {
        std::cout << "Edge> ";
        std::string input;
        std::getline(std::cin, input);
        
        // 检查是否结束输入
        if (input == "done" || input == "exit") {
            break;
        }
        
        // 跳过空行
        if (input.empty()) {
            continue;
        }
        
        // 解析输入,将字符串转换为输入流
        std::istringstream iss(input);
        Name src, dst;
        Value weight;
        
        // 读取并验证输入格式
        if (!(iss >> src >> dst >> weight)) {
            std::cout << "Error: Invalid input format. Please enter in the format: SourceNode DestinationNode Weight\n";
            continue;
        }
        
        src = toupper(src);
        dst = toupper(dst);

/*        // 验证节点是否存在
        if (!graph_.nameToIndex_.count(src)) {
            std::cout << "Error: Source node " << src << " does not exist.\n";
            continue;
        }
        if (!graph_.nameToIndex_.count(dst)) {
            std::cout << "Error: Destination node " << dst << " does not exist.\n";
            continue;
        }
*/

        // 验证权重是否为正值
        if (weight <= 0) {
            std::cout << "Error: Weight must be a positive number.\n";
            continue;
        }

        try {
            int result = graph_.addEdge(src, dst, weight);
            if (result == 0) {
                std::cout << "Edge (" << src << ", " << dst << ") added successfully.\n";
            } else if (result == 2) {
                std::cout << "Edge (" << src << ", " << dst << ") already exists.\n";
            } else if (result == 1){
                PathS pathFinder(graph_);
                pathFinder.searchPath();
                pathFinder.printPathto(dst, src, weight,1);// 1表示打印环路
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}


void Console::findShortestPath() {
    std::cout << "Enter start node and end node (e.g. A B): ";
    Name start, end;
    std::cin >> start >> end;
    std::cin.ignore();  // 清除输入缓冲区
    
    start = toupper(start);
    end = toupper(end);

    Dijkstra dijkstra(graph_);
    dijkstra.shortestPath(start);
    
    Value distance = dijkstra.getDistance(end);
    if (distance == INF) {
        std::cout << "No path exists from " << start << " to " << end << ".\n";
        return;
    }
    
    std::cout << "Shortest distance from " << start << " to " << end << ": " << distance << "\n";
    std::cout << "Path: ";
    
    auto path = dijkstra.getPath(end);
    for (size_t i = 0; i < path.size(); ++i) {
        if (i != 0) std::cout << " -> ";
        std::cout << path[i];
    }
    std::cout << "\n";
}

void Console::findAllPaths() {
    PathS pathFinder(graph_);
    pathFinder.searchPath();
    pathFinder.printAllPaths();
}

void Console::printGraph() const {
    graph_.printGraph();
}

void Console::findAllShortestPaths() {
    std::cout << "Calculating all shortest paths in the graph...\n";
    
    // 遍历所有节点作为起点
    for (const auto& startNode : graph_.nodes_) {
        Dijkstra dijkstra(graph_);
        dijkstra.shortestPath(startNode.name);
        
        std::cout << "\nShortest paths from node " << startNode.name << ":\n";
        
        // 遍历所有节点作为终点
        for (const auto& endNode : graph_.nodes_) {
            if (startNode.name == endNode.name) continue; // 跳过自身
            
            Value distance = dijkstra.getDistance(endNode.name);
            
            if (distance == INF) {
                std::cout << "  No path to " << endNode.name << "\n";
                continue;
            }
            
            std::cout << "  To " << endNode.name << ": " << distance << " (";
            
            auto path = dijkstra.getPath(endNode.name);
            for (size_t i = 0; i < path.size(); ++i) {
                if (i != 0) std::cout << " -> ";
                std::cout << path[i];
            }
            std::cout << ")\n";
        }
    }
    
    std::cout << "All shortest paths calculation completed.\n";
}

void Console::deleteNode() {
    Name name;
    std::cout << "Enter node name to delete: ";
    std::cin >> name;
    std::cin.ignore();  // 清除输入缓冲区
    
    name = toupper(name);
    
    int result = graph_.removeNode(name);
    if (result == 0) {
        std::cout << "Node " << name << " deleted successfully.\n";
    } else {
        std::cout << "Node " << name << " does not exist.\n";
    }
    return;
}

void Console::deleteEdge() {
    // 验证节点是否存在
    Name src, dst;
    std::cout << "Enter source and destination nodes of edge to delete (e.g. A B): ";
    std::cin >> src >> dst;
    std::cin.ignore();  // 清除输入缓冲区
    
    src = toupper(src);
    dst = toupper(dst);
    // 验证边是否存在
    int result = graph_.removeEdge(src, dst);

    if (result == 0) {
        std::cout << "Edge (" << src << ", " << dst << ") deleted successfully.\n";
    } else if (result == 1) {
        std::cout << "Edge (" << src << ", " << dst << ") does not exist.\n";
    } else if (result == 2) {
        std::cout << "errEdge (" << src << ", " << dst << ") deleted successfully.\n";
    }
    return;
}

void Console::printPath() {
    // 验证节点是否存在
    Name start, end;
    std::cout << "Enter start and end nodes of path (e.g. A B): ";
    std::cin >> start >> end;
    std::cin.ignore();  // 清除输入缓冲区
    
    start = toupper(start);
    end = toupper(end);
    // 验证边是否存在
    PathS pathFinder(graph_);
    pathFinder.searchPath();

    pathFinder.printPathto(start, end, 0,0);
    std::cout << "\n";    
}

void Console::printCicle() {
    // 验证节点是否存在
    PathS pathFinder(graph_);
    pathFinder.searchPath();
    bool flag = false;
    for (const auto& ernode : graph_.errnodes_) {
        //遍历所有节点
        if (!ernode.edges.empty()) {
            flag = true;// 存在cycle
            std::cout << "Node: " << ernode.name << " has cycles:\n";
            //查找节点是否有环路
            for (const auto& edge : ernode.edges) {
                pathFinder.printPathto(edge.first, ernode.name, edge.second,1);
            }
            std::cout << "\n";
        }
    }

    if (!flag) {
        std::cout << "There is no cycle in the graph.\n";
    } else {
        std::cout << "All cycles have been printed.\n";
    }
    return;
}