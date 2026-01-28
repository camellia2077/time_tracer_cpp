// domain/ports/file_system_interface.hpp
// 领域层接口：文件系统读写的契约定义
#pragma once
#ifndef DOMAIN_PORTS_FILE_SYSTEM_INTERFACE_H_
#define DOMAIN_PORTS_FILE_SYSTEM_INTERFACE_H_

#include <string>
#include <filesystem>
#include <optional>

namespace domain::ports {

/**
 * @interface FileReaderInterface
 * @brief 文件读取接口，定义读取配置或数据的能力
 */
class FileReaderInterface {
public:
    virtual ~FileReaderInterface() = default;

    /**
     * @brief 读取文件内容
     * @param file_path 文件路径
     * @return 文件内容，如果读取失败返回 nullopt
     */
    virtual std::optional<std::string> ReadFile(const std::filesystem::path& file_path) = 0;
};

/**
 * @interface FileWriterInterface
 * @brief 文件写入接口，定义输出结果和管理目录的能力
 */
class FileWriterInterface {
public:
    virtual ~FileWriterInterface() = default;

    /**
     * @brief 设置输出目录结构
     * @param master_dir 主目录名
     * @param start_year 起始年份
     * @param end_year 结束年份
     * @return 是否成功
     */
    virtual bool SetupDirectories(const std::string& master_dir, int start_year, int end_year) = 0;

    /**
     * @brief 写入日志文件
     * @param file_path 文件路径
     * @param content 文件内容
     * @return 是否成功
     */
    virtual bool WriteLogFile(const std::filesystem::path& file_path, const std::string& content) = 0;
};

}  // namespace domain::ports

#endif  // DOMAIN_PORTS_FILE_SYSTEM_INTERFACE_H_
