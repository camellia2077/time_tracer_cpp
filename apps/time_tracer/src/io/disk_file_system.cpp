// io/disk_file_system.cpp
#include "io/disk_file_system.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>

// 保持对 ANSI Colors 的引用用于错误输出，或者也可以改为抛出异常让上层处理
#include "common/ansi_colors.hpp" 

namespace fs = std::filesystem;

namespace io {

// --- 读取操作 ---

std::string DiskFileSystem::read_content(const fs::path& path) {
    if (!fs::exists(path)) {
        throw std::runtime_error("File not found: " + path.string());
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for reading: " + path.string());
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    
    if (file.bad()) {
        throw std::runtime_error("Error occurred while reading file: " + path.string());
    }

    return buffer.str();
}

std::vector<fs::path> DiskFileSystem::find_files(const fs::path& root, const std::string& ext) {
    std::vector<fs::path> files_found;
    
    // 基本检查
    if (!fs::exists(root) || !fs::is_directory(root)) {
        return files_found;
    }

    try {
        for (const auto& entry : fs::recursive_directory_iterator(root)) {
            if (entry.is_regular_file() && entry.path().extension() == ext) {
                files_found.push_back(entry.path());
            }
        }
    } catch (const fs::filesystem_error& e) {
        // 这里选择打印错误，或者也可以选择抛出
        std::cerr << RED_COLOR << "Filesystem error accessing directory " << root << ": " << e.what() << RESET_COLOR << std::endl;
    }

    std::sort(files_found.begin(), files_found.end());
    return files_found;
}

std::vector<std::string> DiskFileSystem::resolve_file_paths(const std::vector<std::string>& inputs, const std::string& ext) {
    std::vector<std::string> resolved_files;
    
    for (const std::string& path_str : inputs) {
        fs::path p(path_str);
        if (!fs::exists(p)) {
            std::cerr << YELLOW_COLOR << "Warning: Path does not exist: " << path_str << RESET_COLOR << std::endl;
            continue;
        }

        if (fs::is_regular_file(p)) {
            if (p.extension() == ext) {
                resolved_files.push_back(p.string());
            }
        } else if (fs::is_directory(p)) {
            // 复用自身的 find_files 方法
            auto found_paths = find_files(p, ext);
            for (const auto& fp : found_paths) {
                resolved_files.push_back(fp.string());
            }
        }
    }
    
    // 去重并排序
    std::sort(resolved_files.begin(), resolved_files.end());
    auto last = std::unique(resolved_files.begin(), resolved_files.end());
    resolved_files.erase(last, resolved_files.end());

    return resolved_files;
}

// --- 写入操作 ---

void DiskFileSystem::write_content(const fs::path& path, const std::string& content) {
    // 确保父目录存在，这是一种防御性编程，也可以让调用者负责
    if (path.has_parent_path() && !fs::exists(path.parent_path())) {
        create_directories(path.parent_path());
    }

    std::ofstream file(path, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for writing: " + path.string());
    }

    file << content;

    if (file.fail()) {
        throw std::runtime_error("Error occurred while writing to file: " + path.string());
    }
}

void DiskFileSystem::create_directories(const fs::path& path) {
    try {
        fs::create_directories(path);
    } catch (const fs::filesystem_error& e) {
        throw std::runtime_error("FileSystem Error: Failed to create directories at '" + path.string() + "'. Reason: " + e.what());
    }
}

// --- 状态检查 ---

bool DiskFileSystem::exists(const fs::path& path) {
    // std::filesystem::exists 不抛出异常，只返回 false
    std::error_code ec;
    return fs::exists(path, ec);
}

bool DiskFileSystem::is_directory(const fs::path& path) {
    std::error_code ec;
    return fs::is_directory(path, ec);
}

bool DiskFileSystem::is_regular_file(const fs::path& path) {
    std::error_code ec;
    return fs::is_regular_file(path, ec);
}

} // namespace io