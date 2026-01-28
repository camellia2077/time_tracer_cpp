// application/handlers/export_handler.hpp
// 报表导出处理�?- Command Handler 模式
// 内部调用现有 ReportRepository 实现
#ifndef APPLICATION_HANDLERS_EXPORT_HANDLER_HPP_
#define APPLICATION_HANDLERS_EXPORT_HANDLER_HPP_

#include "application/commands/export_command.hpp"
#include "core/domain/ports/i_file_system.hpp"
#include "core/domain/ports/i_user_notifier.hpp"
#include "core/domain/repositories/i_report_repository.hpp"
#include <filesystem>
#include <memory>
#include <string>

namespace fs = std::filesystem;

namespace core::handlers {

class ExportHandler {
public:
  ExportHandler(
      std::shared_ptr<core::interfaces::IFileSystem> file_system,
      std::shared_ptr<core::interfaces::IUserNotifier> notifier,
      std::shared_ptr<core::domain::repositories::IReportRepository> repository,
      const fs::path &output_root);

  ~ExportHandler();

  // 执行导出命令
  core::commands::ExportResult
  Handle(const core::commands::ExportCommand &command);

private:
  std::shared_ptr<core::interfaces::IFileSystem> file_system_;
  std::shared_ptr<core::interfaces::IUserNotifier> notifier_;
  std::shared_ptr<core::domain::repositories::IReportRepository> repository_;
  fs::path output_root_;

  // 内部辅助方法
  std::string GenerateReport(const core::commands::ExportCommand &command);
  fs::path BuildOutputPath(const core::commands::ExportCommand &command);
};

} // namespace core::handlers

#endif // APPLICATION_HANDLERS_EXPORT_HANDLER_HPP_
