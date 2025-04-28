#ifndef CONSOLE_H
#define CONSOLE_H

#include "res.h"
#include "Dijk.h"
#include "PathS.h"

namespace TASK1 {

class Console {
public:
    Console(ResNet& graph);
    void start();  // 启动命令行交互
    ~Console();
private:
    void printHelp() const;  // 打印帮助信息
    void processCommand(const std::string& cmd);  // 处理命令
    void addNode();  // 添加节点
    void addEdge();  // 添加边
    void findShortestPath();  // 查找最短路径
    void findAllPaths();  // 查找所有路径
    void printGraph() const;  // 打印图结构
    void findAllShortestPaths();  // 查找所有最短路径
    void deleteNode();  // 删除节点
    void deleteEdge();  // 删除边
    void printPath();
    void printCicle();
    void printErrEdge();
private:
    ResNet& graph_;  // 图的引用
};

} // namespace TASK1

#endif // CONSOLE_H
