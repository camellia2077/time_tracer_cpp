// cli/framework/console_io.cpp
// 合并自: console_input.cpp, console_notifier.cpp
#include "cli/framework/console_io.hpp"
#include "common/ansi_colors.hpp"
#include <sstream>
#include <limits>

// ============================================================================
// ConsoleInput Implementation
// ============================================================================

std::vector<std::string> ConsoleInput::prompt_for_paths(const std::string& message) const {
    std::cout << message;
    
    std::string line;
    // 使用 getline 读取整行，支持空格分隔的多个路径
    if (std::cin.peek() == '\n') std::cin.ignore(); // 防止之前的残留换行符影响
    std::getline(std::cin, line);
    
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> user_inputs;
    while (ss >> token) {
        user_inputs.push_back(token);
    }
    return user_inputs;
}

bool ConsoleInput::prompt_confirmation(const std::string& message) const {
    std::cout << message << " (y/n): ";
    char confirmation;
    std::cin >> confirmation;

    // 清除输入缓冲区的剩余字符（包括换行符），防止影响后续的 getline 调用
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (confirmation == 'y' || confirmation == 'Y') {
        return true;
    }
    return false;
}

// ============================================================================
// ConsoleNotifier Implementation
// ============================================================================

void ConsoleNotifier::notify_info(const std::string& message) {
    std::cout << message << std::endl;
}

void ConsoleNotifier::notify_success(const std::string& message) {
    std::cout << GREEN_COLOR << message << RESET_COLOR << std::endl;
}

void ConsoleNotifier::notify_warning(const std::string& message) {
    std::cout << YELLOW_COLOR << message << RESET_COLOR << std::endl;
}

void ConsoleNotifier::notify_error(const std::string& message) {
    std::cerr << RED_COLOR << message << RESET_COLOR << std::endl;
}
