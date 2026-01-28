// application/handlers/export_handler.cpp
// 报表导出处理器实�?
#include "application/handlers/export_handler.hpp"
#include <stdexcept>

namespace core::handlers {

ExportHandler::ExportHandler(
    std::shared_ptr<core::interfaces::IFileSystem> file_system,
    std::shared_ptr<core::interfaces::IUserNotifier> notifier,
    std::shared_ptr<core::domain::repositories::IReportRepository> repository,
    const fs::path &output_root)
    : file_system_(std::move(file_system)), notifier_(std::move(notifier)),
      repository_(std::move(repository)), output_root_(output_root) {}

ExportHandler::~ExportHandler() = default;

core::commands::ExportResult
ExportHandler::Handle(const core::commands::ExportCommand &command) {
  core::commands::ExportResult result;

  notifier_->NotifyInfo("--- 开始生成报?---");

  try {
    // 1. 生成报表内容
    result.content = GenerateReport(command);

    // 2. 确定输出路径
    result.output_file = BuildOutputPath(command);

    // 3. 写入文件
    file_system_->CreateDirectories(result.output_file.parent_path());
    file_system_->WriteContent(result.output_file, result.content);

    notifier_->NotifySuccess("报表已生? " + result.output_file.string());

  } catch (const std::exception &e) {
    result.success = false;
    result.errors.push_back(e.what());
    notifier_->NotifyError("报表生成失败: " + std::string(e.what()));
  }

  return result;
}

std::string
ExportHandler::GenerateReport(const core::commands::ExportCommand &command) {
  switch (command.type) {
  case core::commands::ReportType::Daily:
    return repository_->GetDailyReport(command.date_range.start_date,
                                       command.format);
  case core::commands::ReportType::Weekly:
    // TODO: 解析 year ?week ?date_range
    return repository_->GetWeeklyReport(2024, 1, command.format);
  case core::commands::ReportType::Monthly:
    return repository_->GetMonthlyReport(
        command.date_range.start_date.substr(0, 7), command.format);
  case core::commands::ReportType::Recent:
    // TODO: 计算天数
    return repository_->GetRecentReport(30, command.format);
  case core::commands::ReportType::Range:
    return repository_->GetRangeReport(command.date_range.start_date,
                                       command.date_range.end_date,
                                       command.format);
  default:
    throw std::invalid_argument("Unknown report type");
  }
}

fs::path
ExportHandler::BuildOutputPath(const core::commands::ExportCommand &command) {
  std::string ext;
  switch (command.format) {
  case ReportFormat::Markdown:
    ext = ".md";
    break;
  case ReportFormat::LaTeX:
    ext = ".tex";
    break;
  case ReportFormat::Typ:
    ext = ".typ";
    break;
  default:
    ext = ".txt";
    break;
  }

  std::string filename = "report_" + command.date_range.start_date + ext;
  return command.output_path.empty() ? output_root_ / filename
                                     : command.output_path / filename;
}

} // namespace core::handlers
