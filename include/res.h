#ifndef RES_H
#define RES_H

#include <vector>
#include <memory>

namespace TASK1 {

    typedef std::vector<int> IntVector ;//int型动态数组
    typedef std::vector<double> DoubleVector ;//double型动态数组
    typedef int Value ;//int型变量

   
    
    class ListNode {
        public:
            Value value;
            std::shared_ptr<ListNode> next;
            ListNode(Value value) : value(value), next(nullptr) {}
            ~ListNode() {}
        
    };

    class ResNet {
    public:
        ResNet();
        virtual ~ResNet();

        void add(const Value& value);
        void printList() const;
        int size() const;

    private:
        int size_;
        std::shared_ptr<ListNode> head;
        std::shared_ptr<ListNode> tail;  
    };

    // 声明为inline函数，避免头文件中出现重复定义

}

#endif // RES_H

