#include "res.h"
#include <iostream>

using namespace TASK1;

ResNet::ResNet() : head(nullptr), tail(nullptr) {
    std::cout << "RES constructor called" << std::endl;
}

ResNet::~ResNet() {
    std::cout << "RES destructor called" << std::endl;
}

void ResNet::add(const Value& value) { // 使用常量引用避免拷贝
    auto newNode = std::make_shared<ListNode>(value);
    
    if(!head) {
        head = newNode;
        tail = newNode;
    }
    else {
        tail->next = newNode;
        tail = newNode;
    }

    this->size_++;
}

void ResNet::printList() const { // 声明为const方法，因为不修改对象状态
    auto current = head;
    while(current) {
        std::cout << current->value << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

int ResNet::size() const { // 声明为const方法，因为不修改对象状态
    return this->size_;
}


