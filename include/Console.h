#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>

class Console {
    public:
    static void println(const std::string& message) {
        std::cout << message << std::endl;
        }
};


#endif // CONSOLE_H