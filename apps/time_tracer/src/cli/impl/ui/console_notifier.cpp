// cli/impl/ui/console_notifier.cpp
#include "cli/impl/ui/console_notifier.hpp"
#include "common/ansi_colors.hpp"

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