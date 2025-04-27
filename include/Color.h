// include/Color.h
#ifndef COLOR_H
#define COLOR_H

#include <string>

namespace TASK1 {

class Color {
public:
    // 前景色
    static const std::string BLACK;
    static const std::string RED;
    static const std::string GREEN;
    static const std::string YELLOW;
    static const std::string BLUE;
    static const std::string MAGENTA;
    static const std::string CYAN;
    static const std::string WHITE;
    
    // 背景色
    static const std::string BG_BLACK;
    static const std::string BG_RED;
    static const std::string BG_GREEN;
    static const std::string BG_YELLOW;
    static const std::string BG_BLUE;
    static const std::string BG_MAGENTA;
    static const std::string BG_CYAN;
    static const std::string BG_WHITE;
    
    // 样式
    static const std::string RESET;
    static const std::string BOLD;
    static const std::string UNDERLINE;
    static const std::string INVERSE;
    
    // 组合方法
    static std::string colorize(const std::string& text, 
                               const std::string& fg_color,
                               const std::string& style = "");
};

} // namespace TASK1

#endif // COLOR_H
