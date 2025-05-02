#ifndef CONSOLE_H
#define CONSOLE_H

#include "res.h"
#include "Dijk.h"
#include "PathS.h"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace TASK1 {

class Console {
public:
    Console(ResNet& graph);
    ~Console() = default;

    // 执行命令并返回JSON结果
    std::string executeCommand(const std::string& cmd);

    // JSON格式化工具
    static std::string formatJsonOutput(const std::string& type, 
                                      const std::string& message = "",
                                      const std::vector<std::string>& nodes = {},
                                      const std::vector<std::pair<std::string, std::string>>& edges = {});

private:
    // 命令处理方法
    std::string processCommand(const std::string& cmd);
    std::string getHelp();
    std::string addNode(const std::string& nodeName);
    std::string addEdge(const std::string& srcStr, const std::string& dstStr, double weight);
    std::string findShortestPath(const std::string& startStr, const std::string& endStr);
    std::string findAllShortestPaths();
    std::string printGraph();
    std::string deleteNode(const std::string& nodeName);
    std::string deleteEdge(const std::string& srcStr, const std::string& dstStr);
    std::string printPath(const std::string& startStr, const std::string& endStr);
    std::string printCycle();
    std::string printErrEdge();

    ResNet& graph_;  // 图的引用
};

} // namespace TASK1

#endif // CONSOLE_H
