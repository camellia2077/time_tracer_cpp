// core/application/interfaces/i_file_system.hpp
#ifndef CORE_APPLICATION_INTERFACES_I_FILE_SYSTEM_HPP_
#define CORE_APPLICATION_INTERFACES_I_FILE_SYSTEM_HPP_

#include <string>
#include <vector>
#include <filesystem>

namespace core::interfaces {

class IFileSystem {
public:
    virtual ~IFileSystem() = default;

    // --- 读取操作 ---
    virtual std::string read_content(const std::filesystem::path& path) = 0;
    
    // 递归查找指定后缀的文件
    virtual std::vector<std::filesystem::path> find_files(const std::filesystem::path& root, const std::string& ext) = 0;
    
    // 解析混合路径列表（文件+目录），返回所有符合后缀的文件路径
    virtual std::vector<std::string> resolve_file_paths(const std::vector<std::string>& inputs, const std::string& ext) = 0;

    // --- 写入操作 ---
    virtual void write_content(const std::filesystem::path& path, const std::string& content) = 0;
    virtual void create_directories(const std::filesystem::path& path) = 0;
    
    // --- 状态检查 ---
    virtual bool exists(const std::filesystem::path& path) = 0;
    virtual bool is_directory(const std::filesystem::path& path) = 0;
    virtual bool is_regular_file(const std::filesystem::path& path) = 0;
};

}
#endif