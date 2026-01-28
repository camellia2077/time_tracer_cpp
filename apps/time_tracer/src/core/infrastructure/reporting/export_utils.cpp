// core/infrastructure/reporting/export_utils.cpp
#include "core/infrastructure/reporting/export_utils.hpp"
#include <filesystem>

namespace fs = std::filesystem;

namespace ExportUtils {

std::optional<ReportFormatDetails>
GetReportFormatDetails(ReportFormat format,
                       core::interfaces::IUserNotifier *notifier) {
  switch (format) {
  case ReportFormat::Markdown:
    return ReportFormatDetails{"Markdown_logs", ".md"};
  case ReportFormat::LaTeX:
    return ReportFormatDetails{"LaTeX_logs", ".tex"};
  case ReportFormat::Typ:
    return ReportFormatDetails{"Typst_logs", ".typ"};
  default:
    if (notifier)
      notifier->NotifyError("错误: 不支持的导出格式。");
    return std::nullopt;
  }
}

void ExecuteExportTask(const std::string &report_type_name_singular,
                       const fs::path &export_root_path,
                       core::interfaces::IUserNotifier &notifier,
                       const std::function<int()> &file_writing_lambda) {
  try {
    int files_created = file_writing_lambda();

    if (files_created > 0) {
      fs::path final_path = fs::absolute(export_root_path);
      notifier.NotifySuccess("成功: 共创建 " + std::to_string(files_created) +
                             " 个" + report_type_name_singular +
                             "文件，已保存至: " + final_path.string());
    } else {
      notifier.NotifyWarning("信息: 没有可导出的" + report_type_name_singular +
                             "内容。");
    }

  } catch (const std::exception &e) {
    notifier.NotifyError("导出过程中发生错误: " + std::string(e.what()));
  }
}

} // namespace ExportUtils
