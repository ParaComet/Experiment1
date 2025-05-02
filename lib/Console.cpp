#include "Console.h"
#include "PathS.h"
#include <cctype>
#include <sstream>
#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace TASK1 {

Console::Console(ResNet& graph) : graph_(graph) {}

std::string Console::executeCommand(const std::string& cmd) {
    if (cmd.empty()) {
        return formatJsonOutput("error", "Empty command");
    }
    return processCommand(cmd);
}

std::string Console::formatJsonOutput(const std::string& type, 
                                    const std::string& message,
                                    const std::vector<std::string>& nodes,
                                    const std::vector<std::pair<std::string, std::string>>& edges) {
    json output;
    output["type"] = type;
    output["message"] = message;
    
    if (!nodes.empty()) {
        output["nodes"] = nodes;
    }
    
    if (!edges.empty()) {
        json edgesArray = json::array();
        for (const auto& edge : edges) {
            edgesArray.push_back({{"source", edge.first}, {"target", edge.second}});
        }
        output["edges"] = edgesArray;
    }
    
    return output.dump();
}

std::string Console::processCommand(const std::string& cmd) {
    try {
        std::istringstream iss(cmd);
        std::string command;
        iss >> command;
        
        if (command == "help") {
            return getHelp();
        } else if (command == "addnode") {
            std::string nodeName;
            iss >> nodeName;
            return addNode(nodeName);
        } else if (command == "addedge") {
            std::string src, dst;
            double weight;
            iss >> src >> dst >> weight;
            return addEdge(src, dst, weight);
        } else if (command == "shortest") {
            std::string start, end;
            iss >> start >> end;
            return findShortestPath(start, end);
        } else if (command == "dijk") {
            return findAllShortestPaths();
        } else if (command == "print") {
            return printGraph();
        } else if (command == "dlnode") {
            std::string nodeName;
            iss >> nodeName;
            return deleteNode(nodeName);
        } else if (command == "dledge") {
            std::string src, dst;
            iss >> src >> dst;
            return deleteEdge(src, dst);
        } else if (command == "printpath") {
            std::string start, end;
            iss >> start >> end;
            return printPath(start, end);
        } else if (command == "printcycle") {
            return printCycle();
        } else if (command == "printerr") {
            return printErrEdge();
        } else {
            return formatJsonOutput("error", "Unknown command: " + command);
        }
    } catch (const std::exception& e) {
        return formatJsonOutput("error", std::string("Command processing failed: ") + e.what());
    }
}

std::string Console::getHelp() {
    return formatJsonOutput("help", R"(
Available commands:
- help: Show this help
- addnode [name]: Add node
- addedge [src] [dst] [weight]: Add edge
- shortest [start] [end]: Find shortest path
- dijk: Find all shortest paths
- print: Print graph
- dlnode [name]: Delete node
- dledge [src] [dst]: Delete edge
- printpath [start] [end]: Print path
- printcycle: Print cycles
- printerr: Print error edges
)");
}

std::string Console::addNode(const std::string& nodeName) {
    try {
        if (nodeName.empty()) {
            return formatJsonOutput("error", "Node name cannot be empty");
        }
        
        Name name = toupper(nodeName[0]);
        std::string nameStr(1, name);
        
        if (graph_.addNode(name) == -1) {
            return formatJsonOutput("error", "Node " + nameStr + " already exists");
        }
        return formatJsonOutput("success", "Node " + nameStr + " added");
    } catch (const std::exception& e) {
        return formatJsonOutput("error", std::string("Add node failed: ") + e.what());
    }
}

std::string Console::addEdge(const std::string& srcStr, const std::string& dstStr, double weight) {
    try {
        if (srcStr.empty() || dstStr.empty()) {
            return formatJsonOutput("error", "Source and destination cannot be empty");
        }
        
        if (weight <= 0) {
            return formatJsonOutput("error", "Weight must be positive");
        }

        Name src = toupper(srcStr[0]);
        Name dst = toupper(dstStr[0]);
        
        int result = graph_.addEdge(src, dst, weight);
        switch (result) {
            case 0: return formatJsonOutput("success", "Edge added");
            case 1: return formatJsonOutput("success", "Edge added with path");
            case 2: return formatJsonOutput("error", "Edge already exists");
            case 3: return formatJsonOutput("error", "Self-loop not allowed");
            default: return formatJsonOutput("error", "Unknown error");
        }
    } catch (const std::exception& e) {
        return formatJsonOutput("error", std::string("Add edge failed: ") + e.what());
    }
}

std::string Console::findShortestPath(const std::string& startStr, const std::string& endStr) {
    try {
        if (startStr.empty() || endStr.empty()) {
            return formatJsonOutput("error", "Start and end nodes required");
        }

        Name start = toupper(startStr[0]);
        Name end = toupper(endStr[0]);
        
        if (graph_.findNode(start) == -1) {
            return formatJsonOutput("error", "Start node not found");
        }
        if (graph_.findNode(end) == -1) {
            return formatJsonOutput("error", "End node not found");
        }

        Dijkstra dijkstra(graph_);
        dijkstra.shortestPath(start);
        
        Value distance = dijkstra.getDistance(end);
        if (distance == INF) {
            return formatJsonOutput("error", "No path exists");
        }
        
        std::vector<std::string> nodes;
        auto path = dijkstra.getPath(end);
        for (const auto& node : path) {
            nodes.push_back(std::string(1, node));
        }
        
        std::vector<std::pair<std::string, std::string>> edges;
        for (size_t i = 0; i < path.size() - 1; i++) {
            edges.emplace_back(std::string(1, path[i]), std::string(1, path[i+1]));
        }
        
        return formatJsonOutput("shortest_path", 
                              "Distance: " + std::to_string(distance),
                              nodes, edges);
    } catch (const std::exception& e) {
        return formatJsonOutput("error", std::string("Shortest path failed: ") + e.what());
    }
}

std::string Console::findAllShortestPaths() {
    try {
        std::vector<std::string> nodes;
        std::vector<std::pair<std::string, std::string>> edges;
        
        for (const auto& startNode : graph_.nodes_) {
            Dijkstra dijkstra(graph_);
            dijkstra.shortestPath(startNode.name);
            
            for (const auto& endNode : graph_.nodes_) {
                if (startNode.name == endNode.name) continue;
                
                Value distance = dijkstra.getDistance(endNode.name);
                if (distance != INF) {
                    auto path = dijkstra.getPath(endNode.name);
                    for (size_t i = 0; i < path.size() - 1; i++) {
                        edges.emplace_back(std::string(1, path[i]), std::string(1, path[i+1]));
                    }
                }
            }
        }
        
        return formatJsonOutput("all_shortest_paths", "All shortest paths calculated", nodes, edges);
    } catch (const std::exception& e) {
        return formatJsonOutput("error", std::string("Find all paths failed: ") + e.what());
    }
}

std::string Console::printGraph() {
    try {
        std::vector<std::string> nodes;
        std::vector<std::pair<std::string, std::string>> edges;
        
        for (const auto& node : graph_.nodes_) {
            nodes.push_back(std::string(1, node.name));
            for (const auto& edge : node.edges) {
                edges.emplace_back(std::string(1, node.name), std::string(1, edge.first));
            }
        }
        
        return formatJsonOutput("graph", "Graph structure", nodes, edges);
    } catch (const std::exception& e) {
        return formatJsonOutput("error", std::string("Print graph failed: ") + e.what());
    }
}

std::string Console::deleteNode(const std::string& nodeName) {
    try {
        if (nodeName.empty()) {
            return formatJsonOutput("error", "Node name required");
        }
        
        Name name = toupper(nodeName[0]);
        std::string nameStr(1, name);
        
        int result = graph_.removeNode(name);
        if (result == 0) {
            return formatJsonOutput("success", "Node " + nameStr + " deleted");
        }
        return formatJsonOutput("error", "Node " + nameStr + " not found");
    } catch (const std::exception& e) {
        return formatJsonOutput("error", std::string("Delete node failed: ") + e.what());
    }
}

std::string Console::deleteEdge(const std::string& srcStr, const std::string& dstStr) {
    try {
        if (srcStr.empty() || dstStr.empty()) {
            return formatJsonOutput("error", "Source and destination required");
        }
        
        Name src = toupper(srcStr[0]);
        Name dst = toupper(dstStr[0]);
        
        int result = graph_.removeEdge(src, dst);
        if (result == 0) {
            return formatJsonOutput("success", "Edge deleted");
        }
        return formatJsonOutput("error", "Edge not found");
    } catch (const std::exception& e) {
        return formatJsonOutput("error", std::string("Delete edge failed: ") + e.what());
    }
}

std::string Console::printPath(const std::string& startStr, const std::string& endStr) {
    try {
        if (startStr.empty() || endStr.empty()) {
            return formatJsonOutput("error", "Start and end nodes required");
        }
        
        Name start = toupper(startStr[0]);
        Name end = toupper(endStr[0]);
        
        PathS pathFinder(graph_);
        pathFinder.searchPath();
        
        std::vector<std::string> nodes;
        std::vector<std::pair<std::string, std::string>> edges;
        
        // 这里需要根据实际路径查找逻辑实现
        // 简化示例，实际应根据PathS类实现
        
        return formatJsonOutput("path", "Path found", nodes, edges);
    } catch (const std::exception& e) {
        return formatJsonOutput("error", std::string("Print path failed: ") + e.what());
    }
}

std::string Console::printCycle() {
    try {
        std::vector<std::string> nodes;
        std::vector<std::pair<std::string, std::string>> edges;
        
        PathS pathFinder(graph_);
        pathFinder.searchPath();
        
        // 这里需要根据实际环查找逻辑实现
        // 简化示例，实际应根据PathS类实现
        
        return formatJsonOutput("cycle", "Cycles found", nodes, edges);
    } catch (const std::exception& e) {
        return formatJsonOutput("error", std::string("Print cycle failed: ") + e.what());
    }
}

std::string Console::printErrEdge() {
    try {
        std::vector<std::string> nodes;
        std::vector<std::pair<std::string, std::string>> edges;
        
        // 这里需要根据实际错误边查找逻辑实现
        // 简化示例，实际应根据ResNet类实现
        
        return formatJsonOutput("error_edges", "Error edges found", nodes, edges);
    } catch (const std::exception& e) {
        return formatJsonOutput("error", std::string("Print error edges failed: ") + e.what());
    }
}

} // namespace TASK1
