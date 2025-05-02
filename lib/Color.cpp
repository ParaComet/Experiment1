// lib/Color.cpp
#include "Color.h"

namespace TASK1 {

// ANSI颜色代码定义 - 修改为空字符串
const std::string Color::BLACK = "";
const std::string Color::RED = "";
const std::string Color::GREEN = "";
const std::string Color::YELLOW = "";
const std::string Color::BLUE = "";
const std::string Color::MAGENTA = "";
const std::string Color::CYAN = "";
const std::string Color::WHITE = "";

const std::string Color::BG_BLACK = "";
const std::string Color::BG_RED = "";
const std::string Color::BG_GREEN = "";
const std::string Color::BG_YELLOW = "";
const std::string Color::BG_BLUE = "";
const std::string Color::BG_MAGENTA = "";
const std::string Color::BG_CYAN = "";
const std::string Color::BG_WHITE = "";

const std::string Color::RESET = "";
const std::string Color::BOLD = "";
const std::string Color::UNDERLINE = "";
const std::string Color::INVERSE = "";

/*
const std::string Color::BLACK = "\033[30m";
const std::string Color::RED = "\033[31m";
const std::string Color::GREEN = "\033[32m";
const std::string Color::YELLOW = "\033[33m";
const std::string Color::BLUE = "\033[34m";
const std::string Color::MAGENTA = "\033[35m";
const std::string Color::CYAN = "\033[36m";
const std::string Color::WHITE = "\033[37m";

const std::string Color::BG_BLACK = "\033[40m";
const std::string Color::BG_RED = "\033[41m";
const std::string Color::BG_GREEN = "\033[42m";
const std::string Color::BG_YELLOW = "\033[43m";
const std::string Color::BG_BLUE = "\033[44m";
const std::string Color::BG_MAGENTA = "\033[45m";
const std::string Color::BG_CYAN = "\033[46m";
const std::string Color::BG_WHITE = "\033[47m";

const std::string Color::RESET = "\033[0m";
const std::string Color::BOLD = "\033[1m";
const std::string Color::UNDERLINE = "\033[4m";
const std::string Color::INVERSE = "\033[7m";
*/

std::string Color::colorize(const std::string& text, 
                           const std::string& fg_color,
                           const std::string& style) {
    return text; // 直接返回原始文本
}

} // namespace TASK1
