// cli/framework/help_formatter.cpp
#include "cli/framework/help_formatter.hpp"
#include "common/ansi_colors.hpp"
#include <iostream>
#include <iomanip>

namespace Cli::Framework {

    HelpFormatter::HelpFormatter(std::string prog_name) 
        : prog_name_(std::move(prog_name)) {}

    void HelpFormatter::print_help(const std::vector<Option>& options) const {
        std::cout << GREEN_COLOR << "Usage: " << RESET_COLOR << prog_name_ << " [options]\n\n";
        std::cout << GREEN_COLOR << "Options:\n" << RESET_COLOR;
        
        for (const auto& opt : options) {
            std::cout << "  " << std::left << std::setw(6) << opt.short_name 
                      << std::setw(20) << opt.long_name 
                      << opt.description << "\n";
        }
        std::cout << "\n";
    }

    void HelpFormatter::print_error(const std::string& msg) const {
        std::cerr << RED_COLOR << "Error: " << RESET_COLOR << msg << "\n\n";
    }

}