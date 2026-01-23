// io/file_manager.hpp
#ifndef IO_FILE_MANAGER_HPP_
#define IO_FILE_MANAGER_HPP_

#include "io/file_system_interfaces.hpp" // 引入接口
#include <string>
#include <filesystem>
#include <optional>

// [修改] 显式继承接口
class FileManager : public IFileWriter, public IFileReader {
public:
    // --- IFileWriter 实现 ---
    bool setup_directories(const std::string& master_dir, int start_year, int end_year) override;
    bool write_log_file(const std::filesystem::path& file_path, const std::string& content) override;

    // --- IFileReader 实现 ---
    std::optional<std::string> read_file(const std::filesystem::path& file_path) override;
};

#endif // IO_FILE_MANAGER_HPP_