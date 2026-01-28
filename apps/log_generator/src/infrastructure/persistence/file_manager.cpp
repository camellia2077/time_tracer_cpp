// infrastructure/persistence/file_manager.cpp
// 基础设施层：文件管理器实现
#include "infrastructure/persistence/file_manager.hpp"
#include "cli/framework/ansi_colors.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace infrastructure::persistence {

bool FileManager::SetupDirectories(const std::string& master_dir, int start_year, int end_year) {
    try {
        if (!std::filesystem::exists(master_dir)) {
            std::filesystem::create_directory(master_dir);
            std::cout << "Created master directory: '" << master_dir << "'\n";
        }
        for (int year = start_year; year <= end_year; ++year) {
            std::filesystem::path year_dir_path = std::filesystem::path(master_dir) / std::to_string(year);
            if (!std::filesystem::exists(year_dir_path)) {
                std::filesystem::create_directory(year_dir_path);
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << RED_COLOR << "Error creating directories. Detail: " << e.what() << RESET_COLOR << '\n';
        return false;
    }
    return true;
}

bool FileManager::WriteLogFile(const std::filesystem::path& file_path, const std::string& content) {
    std::ofstream outFile(file_path);
    if (!outFile.is_open()) {
        std::cerr << RED_COLOR << "Error: Could not open file '" << file_path.string() << "' for writing." << RESET_COLOR << '\n';
        return false;
    }
    outFile << content;
    return true;
}

std::optional<std::string> FileManager::ReadFile(const std::filesystem::path& file_path) {
    std::ifstream inFile(file_path);
    if (!inFile.is_open()) {
        std::cerr << RED_COLOR << "Error: Could not open file '" << file_path.string() << "' for reading." << RESET_COLOR << '\n';
        return std::nullopt;
    }
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    return buffer.str();
}

}  // namespace infrastructure::persistence
