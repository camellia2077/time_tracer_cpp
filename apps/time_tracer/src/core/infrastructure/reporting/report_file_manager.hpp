// core/infrastructure/reporting/report_file_manager.hpp
#ifndef CORE_INFRASTRUCTURE_REPORTING_REPORT_FILE_MANAGER_HPP_
#define CORE_INFRASTRUCTURE_REPORTING_REPORT_FILE_MANAGER_HPP_

#include "core/domain/types/report_format.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class ReportFileManager {
public:
  explicit ReportFileManager(const fs::path &export_root);

  fs::path GetSingleDayReportPath(const std::string &date,
                                  ReportFormat format) const;
  fs::path GetSingleWeekReportPath(const std::string &week_name,
                                   ReportFormat format) const;
  fs::path GetSingleMonthReportPath(const std::string &month,
                                    ReportFormat format) const;
  fs::path GetSinglePeriodReportPath(int days, ReportFormat format) const;

  fs::path GetAllDailyReportsBaseDir(ReportFormat format) const;
  fs::path GetAllWeeklyReportsBaseDir(ReportFormat format) const;
  fs::path GetAllMonthlyReportsBaseDir(ReportFormat format) const;
  fs::path GetAllPeriodReportsBaseDir(ReportFormat format) const;

private:
  fs::path export_root_path_;
};

#endif // CORE_INFRASTRUCTURE_REPORTING_REPORT_FILE_MANAGER_HPP_
