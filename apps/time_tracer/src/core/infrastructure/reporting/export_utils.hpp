// core/infrastructure/reporting/export_utils.hpp
#ifndef CORE_INFRASTRUCTURE_REPORTING_EXPORT_UTILS_HPP_
#define CORE_INFRASTRUCTURE_REPORTING_EXPORT_UTILS_HPP_

#include "core/domain/ports/i_user_notifier.hpp"
#include "core/domain/types/report_format.hpp"
#include <filesystem>
#include <functional>
#include <optional>
#include <string>

namespace ExportUtils {

struct ReportFormatDetails {
  std::string dir_name_;
  std::string extension_;
};

// [修改] 增加 notifier 参数 (或者 GetReportFormatDetails 不需要，而
// ExecuteExportTask 需要)
std::optional<ReportFormatDetails>
GetReportFormatDetails(ReportFormat format,
                       core::interfaces::IUserNotifier *notifier = nullptr);

// [修改] 增加 notifier 参数
void ExecuteExportTask(const std::string &report_type_name_singular,
                       const std::filesystem::path &export_root_path,
                       core::interfaces::IUserNotifier &notifier,
                       const std::function<int()> &file_writing_lambda);

} // namespace ExportUtils

#endif
