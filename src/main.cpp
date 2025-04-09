#include <iostream>
#include "res.h"

using namespace TASK1;

void GetData(ResNet &res) {
    int x;
    std::cout << "Enter numbers (Ctrl+Z to end input): ";
    while (std::cin >> x) {
        res.add(x);
    }
}

int main() {
    ResNet myList;
    GetData(myList);
    
    std::cout << "Linked list contents: ";
    myList.printList();
    
    return 0;
}
