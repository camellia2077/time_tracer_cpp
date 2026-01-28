// core/infrastructure/reporting/report_file_manager.cpp
#include "core/infrastructure/reporting/report_file_manager.hpp"
#include "core/infrastructure/reporting/export_utils.hpp"
#include <stdexcept>

ReportFileManager::ReportFileManager(const fs::path &export_root)
    : export_root_path_(export_root) {}

fs::path ReportFileManager::GetSingleDayReportPath(const std::string &date,
                                                   ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  return export_root_path_ / details.dir_name_ / "day" /
         (date + details.extension_);
}

fs::path
ReportFileManager::GetSingleWeekReportPath(const std::string &week_name,
                                           ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  // 路径: export_root/[FormatType]/week/2025-W04.md
  return export_root_path_ / details.dir_name_ / "week" /
         (week_name + details.extension_);
}

fs::path
ReportFileManager::GetSingleMonthReportPath(const std::string &month,
                                            ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  return export_root_path_ / details.dir_name_ / "month" /
         (month + details.extension_);
}

fs::path ReportFileManager::GetSingleYearReportPath(const std::string &year,
                                                    ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  return export_root_path_ / details.dir_name_ / "year" /
         (year + details.extension_);
}

fs::path
ReportFileManager::GetSingleRecentReportPath(int days,
                                             ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  return export_root_path_ / details.dir_name_ / "recent" /
         ("Last_" + std::to_string(days) + "_Days_Report" + details.extension_);
}

fs::path
ReportFileManager::GetSingleRangeReportPath(const std::string &start_date,
                                            const std::string &end_date,
                                            ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  return export_root_path_ / details.dir_name_ / "range" /
         ("Range_" + start_date + "_to_" + end_date + details.extension_);
}

fs::path
ReportFileManager::GetAllDailyReportsBaseDir(ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  return export_root_path_ / details.dir_name_ / "days";
}

fs::path
ReportFileManager::GetAllWeeklyReportsBaseDir(ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  return export_root_path_ / details.dir_name_ / "week";
}

fs::path
ReportFileManager::GetAllMonthlyReportsBaseDir(ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  return export_root_path_ / details.dir_name_ / "month";
}

fs::path
ReportFileManager::GetAllYearlyReportsBaseDir(ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  return export_root_path_ / details.dir_name_ / "year";
}

fs::path
ReportFileManager::GetAllRecentReportsBaseDir(ReportFormat format) const {
  auto details = ExportUtils::GetReportFormatDetails(format).value();
  return export_root_path_ / details.dir_name_ / "recent";
}
