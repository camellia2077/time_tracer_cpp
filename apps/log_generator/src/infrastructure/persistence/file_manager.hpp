// infrastructure/persistence/file_manager.hpp
// 基础设施层：文件管理器实现
#pragma once
#ifndef INFRASTRUCTURE_PERSISTENCE_FILE_MANAGER_H_
#define INFRASTRUCTURE_PERSISTENCE_FILE_MANAGER_H_

#include "domain/ports/file_system_interface.hpp"
#include <string>
#include <filesystem>
#include <optional>

namespace infrastructure::persistence {

/**
 * @class FileManager
 * @brief 文件系统接口的具体实现
 * 
 * 职责：
 * - 读取配置文件内容
 * - 创建输出目录结构
 * - 写入日志文件
 */
class FileManager : public domain::ports::FileWriterInterface, public domain::ports::FileReaderInterface {
public:
    // --- FileWriterInterface 实现 ---
    bool SetupDirectories(const std::string& master_dir, int start_year, int end_year) override;
    bool WriteLogFile(const std::filesystem::path& file_path, const std::string& content) override;

    // --- FileReaderInterface 实现 ---
    std::optional<std::string> ReadFile(const std::filesystem::path& file_path) override;
};

}  // namespace infrastructure::persistence

#endif  // INFRASTRUCTURE_PERSISTENCE_FILE_MANAGER_H_
