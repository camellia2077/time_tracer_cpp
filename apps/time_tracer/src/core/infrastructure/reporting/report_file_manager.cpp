// core/infrastructure/reporting/report_file_manager.cpp
#include "core/infrastructure/reporting/report_file_manager.hpp"
#include "core/infrastructure/reporting/export_utils.hpp"
#include <stdexcept>

ReportFileManager::ReportFileManager(const fs::path &export_root)
    : export_root_path_(export_root) {}

fs::path ReportFileManager::GetSingleDayReportPath(const std::string &date,
                                                   ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  return export_root_path_ / details.dir_name_ / "daily" /
         (date + details.extension_);
}

fs::path
ReportFileManager::GetSingleWeekReportPath(const std::string &week_name,
                                           ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  // 路径: export_root/[FormatType]/weekly/2025-W04.md
  return export_root_path_ / details.dir_name_ / "weekly" /
         (week_name + details.extension_);
}

fs::path
ReportFileManager::GetSingleMonthReportPath(const std::string &month,
                                            ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  return export_root_path_ / details.dir_name_ / "monthly" /
         (month + details.extension_);
}

fs::path
ReportFileManager::GetSinglePeriodReportPath(int days,
                                             ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  return export_root_path_ / details.dir_name_ / "periods" /
         ("Last_" + std::to_string(days) + "_Days_Report" + details.extension_);
}

fs::path
ReportFileManager::GetAllDailyReportsBaseDir(ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  return export_root_path_ / details.dir_name_ / "days";
}

fs::path
ReportFileManager::GetAllWeeklyReportsBaseDir(ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  return export_root_path_ / details.dir_name_ / "weekly";
}

fs::path
ReportFileManager::GetAllMonthlyReportsBaseDir(ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  return export_root_path_ / details.dir_name_ / "monthly";
}

fs::path
ReportFileManager::GetAllPeriodReportsBaseDir(ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  return export_root_path_ / details.dir_name_ / "periods";
}
