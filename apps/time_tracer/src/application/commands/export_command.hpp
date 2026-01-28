// application/commands/export_command.hpp
// 报表导出命令 - CQRS Command 模式
#ifndef APPLICATION_COMMANDS_EXPORT_COMMAND_HPP_
#define APPLICATION_COMMANDS_EXPORT_COMMAND_HPP_

#include "core/domain/types/report_format.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace core::commands {

// ============================================================================
// ReportType - 报表类型
// ============================================================================
enum class ReportType { Daily, Weekly, Monthly, Recent, Range };

// ============================================================================
// DateRange - 日期范围
// ============================================================================
struct DateRange {
  std::string start_date; // YYYY-MM-DD
  std::string end_date;   // YYYY-MM-DD
};

// ============================================================================
// ExportCommand - 导出命令参数
// ============================================================================
struct ExportCommand {
  ReportType type;      // 报表类型
  ReportFormat format;  // 输出格式 (Markdown, LaTeX, Typst)
  DateRange date_range; // 日期范围
  fs::path output_path; // 输出路径
};

// ============================================================================
// ExportResult - 导出命令结果
// ============================================================================
struct ExportResult {
  bool success = true;
  std::string content;  // 生成的报表内�?
  fs::path output_file; // 输出文件路径
  std::vector<std::string> errors;

  bool HasErrors() const { return !errors.empty(); }
};

} // namespace core::commands

#endif // APPLICATION_COMMANDS_EXPORT_COMMAND_HPP_
