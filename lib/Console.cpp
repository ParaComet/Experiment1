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

Console::~Console() {
    
}

void Console::start() {
    std::cout << Color::colorize("Graph Processing Console (type 'help' for commands)", Color::GREEN, Color::BOLD) << std::endl;
    
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
    std::cout << Color::colorize("Available commands:", Color::GREEN, Color::BOLD) << "\n"
              << Color::colorize("  help", Color::YELLOW) << " - Show this help message\n"
              << Color::colorize("  addnode", Color::YELLOW) << " - Add a node to the graph\n"
              << Color::colorize("  addedge", Color::YELLOW) << " - Add multiple edges (enter 'done' when finished)\n"
              << Color::colorize("  shortest", Color::YELLOW) << " - Find shortest path between two nodes\n"
              << Color::colorize("  allpaths", Color::YELLOW) << " - Find all paths in the graph\n"
              << Color::colorize("  dijk", Color::YELLOW) << " - Find all shortest paths in the graph\n"
              << Color::colorize("  print", Color::YELLOW) << " - Print the graph structure\n"
              << Color::colorize("  dlnode", Color::YELLOW) << " - Delete a node from the graph\n"
              << Color::colorize("  dledge", Color::YELLOW) << " - Delete an edge from the graph\n"
              << Color::colorize("  printpath", Color::YELLOW) << " - Print paths from start to end in the graph\n"
              << Color::colorize("  printcycle", Color::YELLOW) << " - Print all cycles in the graph\n"
              << Color::colorize("  exit/quit", Color::YELLOW) << " - Exit the program\n"
              << Color::colorize("  printerr", Color::YELLOW) << " - Print all error edges in the graph\n";
}

void Console::processCommand(const std::string& cmd) {
    using namespace TASK1;
    if (cmd == "help") {
        printHelp();
    } else if (cmd == "addnode") {
        addNode();
    } else if (cmd == "addedge") {
        addEdge();
    } else if (cmd == "shortest") {
        findShortestPath();
    } else if (cmd == "dijk") {
        std::cout << Color::colorize("Calculating all shortest paths in the graph...", Color::CYAN) << "\n";
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
    } else if (cmd == "printcycle") {
        printCicle();
    } else if (cmd == "printerr") {
        printErrEdge();
    } else {
        std::cout << Color::colorize("Unknown command. Type 'help' for available commands.", Color::RED) << "\n";
    }
}

void Console::addNode() {
    using namespace TASK1;
    std::cout << Color::colorize("Enter node name (single character): ", Color::CYAN);
    Name name;
    std::cin >> name;
    std::cin.ignore();
    
    name = toupper(name);
    std::string nameStr(1, name);
    if (graph_.addNode(name) == -1) {
        std::cout << Color::colorize("Node ", Color::RED) 
                  << Color::colorize(nameStr, Color::YELLOW) 
                  << Color::colorize(" already exists.", Color::RED) << "\n";
    } else {
        std::cout << Color::colorize("Node ", Color::GREEN) 
                  << Color::colorize(nameStr, Color::YELLOW) 
                  << Color::colorize(" added successfully.", Color::GREEN) << "\n";
    }
}


void Console::addEdge() {
    using namespace TASK1;
    std::cout << Color::colorize("Enter edges (source destination weight), one per line. Enter 'done' when finished:\n", Color::CYAN);
    
    while (true) {
        std::cout << Color::colorize("Edge> ", Color::BLUE);
        std::string input;
        std::getline(std::cin, input);
        
        if (input == "done" || input == "exit") {
            break;
        }
        
        if (input.empty()) {
            continue;
        }
        
        std::istringstream iss(input);
        Name src, dst;
        Value weight;
        
        //解析输入，若不合法则输出错误信息并继续下一次输入
        if (!(iss >> src >> dst >> weight)) {
            std::cout << Color::colorize("Error: Invalid input format. Please enter in the format: SourceNode DestinationNode Weight\n", Color::RED);
            continue;
        }
        
        src = toupper(src);
        dst = toupper(dst);

        std::string srcStr(1, src);
        std::string dstStr(1, dst);
        
        /*if (graph_.findNode(src) == -1) {
            std::cout << Color::colorize("Error: Source node ", Color::RED)
                      << Color::colorize(srcStr, Color::YELLOW)
                      << Color::colorize(" does not exist.\n", Color::RED);
            continue;
        }
        
        if (graph_.findNode(dst) == -1) {
            std::cout << Color::colorize("Error: Destination node ", Color::RED)
                      << Color::colorize(dstStr, Color::YELLOW)
                      << Color::colorize(" does not exist.\n", Color::RED);
            continue;
        }*/

        // 检测路径权值的合法性
        if (weight <= 0) {
            std::cout << Color::colorize("Error: Weight must be a positive number.\n", Color::RED);
            continue;
        }

        try {
            int result = graph_.addEdge(src, dst, weight);
            if (result == 0) {
                std::cout << Color::colorize("Edge (", Color::GREEN)
                          << Color::colorize(srcStr, Color::YELLOW)
                          << Color::colorize(", ", Color::GREEN)
                          << Color::colorize(dstStr, Color::YELLOW)
                          << Color::colorize(") added successfully.\n", Color::GREEN);
            } else if (result == 2) {
                std::cout << Color::colorize("Edge (", Color::RED)
                          << Color::colorize(srcStr, Color::YELLOW)
                          << Color::colorize(", ", Color::RED)
                          << Color::colorize(dstStr, Color::YELLOW)
                          << Color::colorize(") already exists.\n", Color::RED);
            } else if (result == 1) {
                PathS pathFinder(graph_);
                pathFinder.searchPath();
                pathFinder.printPathto(dst, src, weight, 1);
            } else if (result == 3) {
                 std::cout << Color::colorize("Error: Invalid input self-loop edge.\n", Color::RED);
                 std::cout << Color::colorize(srcStr, Color::YELLOW)
                          << Color::colorize(" -> ", Color::RED)
                          << Color::colorize(dstStr, Color::YELLOW)<< "\n";
            }
        } catch (const std::exception& e) {
            std::cout << Color::colorize("Error: ", Color::RED)
                       << Color::colorize(e.what(), Color::RED) << "\n";
        }
    }
}



void Console::findShortestPath() {
    std::cout << Color::colorize("Enter start node and end node (e.g. A B): ", Color::CYAN);
    Name start, end;
    std::cin >> start >> end;
    std::cin.ignore();  // 清除输入缓冲区
    
    start = toupper(start);
    end = toupper(end);

    std::string startStr(1, start);
    std::string endStr(1, end);
    
    if (graph_.findNode(start) == -1) {
        std::cout << Color::colorize("Error: Start node ", Color::RED)
                  << Color::colorize(startStr, Color::YELLOW)
                  << Color::colorize(" does not exist.\n", Color::RED);
        return;
    }
    
    if (graph_.findNode(end) == -1) {
        std::cout << Color::colorize("Error: End node ", Color::RED)
                  << Color::colorize(endStr, Color::YELLOW)
                  << Color::colorize(" does not exist.\n", Color::RED);
        return;
    }

    Dijkstra dijkstra(graph_);
    dijkstra.shortestPath(start);
    
    Value distance = dijkstra.getDistance(end);
    if (distance == INF) {
        std::cout << Color::colorize("No path exists from ", Color::RED)
                  << Color::colorize(startStr, Color::YELLOW)
                  << Color::colorize(" to ", Color::RED)
                  << Color::colorize(endStr, Color::YELLOW)
                  << Color::colorize(".\n", Color::RED);
        return;
    }
    
    std::cout << Color::colorize("Shortest distance from ", Color::GREEN)
              << Color::colorize(startStr, Color::YELLOW)
              << Color::colorize(" to ", Color::GREEN)
              << Color::colorize(endStr, Color::YELLOW)
              << Color::colorize(": ", Color::GREEN)
              << Color::colorize(std::to_string(distance), Color::MAGENTA) << "\n";
    
    std::cout << Color::colorize("Path: ", Color::CYAN);
    
    auto path = dijkstra.getPath(end);
    for (size_t i = 0; i < path.size(); ++i) {
        if (i != 0) std::cout << Color::colorize(" -> ", Color::BLUE);
        char c = path[i];
        std::string cStr(1, c);
        std::cout << Color::colorize(cStr, Color::YELLOW);
        
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
    std::cout << Color::colorize("Calculating all shortest paths in the graph...\n", Color::CYAN, Color::BOLD);
    
    // 遍历所有节点作为起点
    for (const auto& startNode : graph_.nodes_) {
        Dijkstra dijkstra(graph_);
        dijkstra.shortestPath(startNode.name);
        
        char c = startNode.name;
        std::string cStr(1, c);

        std::cout << "\n" << Color::colorize("Shortest paths from node ", Color::GREEN)
                  << Color::colorize(cStr, Color::YELLOW, Color::BOLD)
                  << Color::colorize(":\n", Color::GREEN);
        
        // 遍历所有节点作为终点
        for (const auto& endNode : graph_.nodes_) {
            if (startNode.name == endNode.name) continue; // 跳过自身
            
            Value distance = dijkstra.getDistance(endNode.name);
            
            char b = endNode.name;
            std::string bStr(1, b);

            if (distance == INF&&startNode.name!=endNode.name) {

                std::cout << Color::colorize("  No path to ", Color::RED)
                          << Color::colorize(bStr, Color::YELLOW) << "\n";
                continue;
            }
            
            std::cout << Color::colorize("  To ", Color::CYAN)
                      << Color::colorize(bStr, Color::YELLOW)
                      << Color::colorize(": ", Color::CYAN)
                      << Color::colorize(std::to_string(distance), Color::MAGENTA)
                      << Color::colorize(" (", Color::CYAN);

            
            auto path = dijkstra.getPath(endNode.name);
            for (size_t i = 0; i < path.size(); ++i) {
                if (i != 0) std::cout << Color::colorize(" -> ", Color::BLUE);
                
                char a = path[i];
                std::string aStr(1, a);
                std::cout << Color::colorize(aStr, Color::YELLOW);
            
            }
            std::cout << Color::colorize(")\n", Color::CYAN);
        }
    }
    
    std::cout << Color::colorize("\nAll shortest paths calculation completed.\n", Color::GREEN, Color::BOLD);
}

void Console::deleteNode() {
    Name name;
    std::cout << Color::colorize("Enter node name to delete: ", Color::CYAN);
    std::cin >> name;
    std::cin.ignore();  // 清除输入缓冲区
    
    name = toupper(name);
    
    char c = name;
    std::string cStr(1, c);
    std::cout << Color::colorize("Deleting node ", Color::RED)
              << Color::colorize(cStr, Color::YELLOW)
              << Color::colorize(" from the graph...\n", Color::RED);

    int result = graph_.removeNode(name);
    if (result == 0) {
        std::cout << Color::colorize("Node ", Color::GREEN)
                  << Color::colorize(cStr, Color::YELLOW)
                  << Color::colorize(" deleted successfully.\n", Color::GREEN);
    } else {
        std::cout << Color::colorize("Node ", Color::RED)
                  << Color::colorize(cStr, Color::YELLOW)
                  << Color::colorize(" does not exist.\n", Color::RED);
    }
    return;
}

void Console::deleteEdge() {
    // 验证节点是否存在
    Name src, dst;
    std::cout << Color::colorize("Enter source and destination nodes of edge to delete (e.g. A B): ", Color::CYAN);
    std::cin >> src >> dst;
    std::cin.ignore();  // 清除输入缓冲区
    
    src = toupper(src);
    dst = toupper(dst);

    char a = src;
    char b = dst;
    std::string aStr(1, a);
    std::string bStr(1, b);

    // 验证边是否存在
    int result = graph_.removeEdge(src, dst);

    if (result == 0) {
        std::cout << Color::colorize("Edge (", Color::GREEN)
                  << Color::colorize(aStr, Color::YELLOW)
                  << Color::colorize(", ", Color::GREEN)
                  << Color::colorize(bStr, Color::YELLOW)
                  << Color::colorize(") deleted successfully.\n", Color::GREEN);
    } else if (result == 1) {
        std::cout << Color::colorize("Edge (", Color::RED)
                  << Color::colorize(aStr, Color::YELLOW)
                  << Color::colorize(", ", Color::RED)
                  << Color::colorize(bStr, Color::YELLOW)
                  << Color::colorize(") does not exist.\n", Color::RED);
    } else if (result == 2) {
        std::cout << Color::colorize("errEdge (", Color::MAGENTA)
                  << Color::colorize(aStr, Color::YELLOW)
                  << Color::colorize(", ", Color::MAGENTA)
                  << Color::colorize(bStr, Color::YELLOW)
                  << Color::colorize(") deleted successfully.\n", Color::MAGENTA);
    }
    return;
}

void Console::printPath() {
    // 验证节点是否存在
    Name start, end;
    std::cout << Color::colorize("Enter start and end nodes of path (e.g. A B): ", Color::CYAN);
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
            char c = ernode.name;
            std::string cStr(1, c);
            std::cout << Color::colorize("Node: ", Color::CYAN)
                      << Color::colorize(cStr, Color::YELLOW, Color::BOLD)
                      << Color::colorize(" has cycles:\n", Color::CYAN);
            //查找节点是否有环路
            for (const auto& edge : ernode.edges) {
                pathFinder.printPathto(edge.first, ernode.name, edge.second,1);
            }
            std::cout << "\n";
        }
    }

    if (!flag) {
        std::cout << Color::colorize("There is no cycle in the graph.\n", Color::GREEN);
    } else {
        std::cout << Color::colorize("All cycles have been printed.\n", Color::GREEN, Color::BOLD);
    }
    return;
}

void Console::printErrEdge() {
    graph_.printErrNodes();
}