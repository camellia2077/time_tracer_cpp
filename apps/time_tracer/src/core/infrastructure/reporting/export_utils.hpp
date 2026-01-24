// core/infrastructure/reporting/export_utils.hpp
#ifndef CORE_REPORTING_EXPORT_EXPORT_UTILS_HPP_
#define CORE_REPORTING_EXPORT_EXPORT_UTILS_HPP_

#include <string>
#include <filesystem>
#include <functional>
#include <optional>
#include "reports/shared/types/report_format.hpp"  
#include "core/application/interfaces/i_user_notifier.hpp"

namespace ExportUtils {

struct ReportFormatDetails {
    std::string dir_name;
    std::string extension;
};

// [修改] 增加 notifier 参数 (或者 get_report_format_details 不需要，但 execute_export_task 需要)
std::optional<ReportFormatDetails> get_report_format_details(ReportFormat format, core::interfaces::IUserNotifier* notifier = nullptr);

// [修改] 增加 notifier 参数
void execute_export_task(
    const std::string& report_type_name_singular,
    const std::filesystem::path& export_root_path,
    core::interfaces::IUserNotifier& notifier,
    const std::function<int()>& file_writing_lambda
);

} 

#endif