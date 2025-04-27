#ifndef RES_H
#define RES_H

#include <unordered_map>
#include <vector>

namespace TASK1 {

    typedef int Value ;//int型变量
    typedef char Name ;
   
    
    class Node {
    public:
        Name name;
        std::vector<std :: pair<Name, Value>> edges;
        int num_edges;
        Node(Name name) : name(name) , num_edges(0) {}
        ~Node() {}
    };


    class ResNet {
    public:
        ResNet();
        virtual ~ResNet();
        int addNode(const Name& name);
        int CircleCheck(const Name& src, const Name& dst);
        int addEdge(const Name& src, const Name& dst, Value value);
        int removeEdge(const Name& src, const Name& dst);
        int removeNode(const Name& name);
        void printGraph() const;
        int findNode(const Name& name) const;

        const std::vector<std::pair<Name, Value>>& getEdges(const Name& node) const;

        std::vector<Node> nodes_;
        std::unordered_map<Name, int> nameToIndex_;
    };

    // 声明为inline函数，避免头文件中出现重复定义

}



#endif // RES_H

