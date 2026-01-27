// application/commands/ingest_command.hpp
// 数据入库命令 - CQRS Command 模式
#ifndef APPLICATION_COMMANDS_INGEST_COMMAND_HPP_
#define APPLICATION_COMMANDS_INGEST_COMMAND_HPP_

#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include "common/types/date_check_mode.hpp"
#include "core/domain/model/daily_log.hpp"

namespace fs = std::filesystem;

namespace core::commands {

// ============================================================================
// IngestCommand - 入库命令参数
// ============================================================================
struct IngestCommand {
    fs::path source_path;                  // 输入源路径（文件或目录）
    DateCheckMode date_check_mode;         // 日期检查模�?
    bool save_processed_output = false;    // 是否保存处理后的输出
};

// ============================================================================
// IngestResult - 入库命令结果
// ============================================================================
struct IngestResult {
    bool success = true;
    std::vector<fs::path> processed_files;
    std::map<std::string, std::vector<DailyLog>> daily_logs;
    std::vector<std::string> errors;
    
    // 便捷方法
    bool has_errors() const { return !errors.empty(); }
    size_t file_count() const { return processed_files.size(); }
};

} // namespace core::commands

#endif // APPLICATION_COMMANDS_INGEST_COMMAND_HPP_
