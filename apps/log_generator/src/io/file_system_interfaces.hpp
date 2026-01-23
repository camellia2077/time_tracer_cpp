// io/file_system_interfaces.hpp
#ifndef IO_FILE_SYSTEM_INTERFACES_HPP_
#define IO_FILE_SYSTEM_INTERFACES_HPP_

#include <string>
#include <filesystem>
#include <optional>

/**
 * @interface IFileReader
 * @brief 仅负责读取配置或数据。
 */
class IFileReader {
public:
    virtual ~IFileReader() = default;
    virtual std::optional<std::string> read_file(const std::filesystem::path& file_path) = 0;
};

/**
 * @interface IFileWriter
 * @brief 仅负责输出结果和管理目录。
 */
class IFileWriter {
public:
    virtual ~IFileWriter() = default;
    virtual bool setup_directories(const std::string& master_dir, int start_year, int end_year) = 0;
    virtual bool write_log_file(const std::filesystem::path& file_path, const std::string& content) = 0;
};

#endif // IO_FILE_SYSTEM_INTERFACES_HPP_