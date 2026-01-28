// core/infrastructure/reporting/exporter.hpp
#ifndef CORE_INFRASTRUCTURE_REPORTING_EXPORTER_HPP_
#define CORE_INFRASTRUCTURE_REPORTING_EXPORTER_HPP_

#include "core/domain/model/query_data_structs.hpp"
#include "core/domain/ports/i_file_system.hpp"
#include "core/domain/ports/i_user_notifier.hpp" // [新增]
#include "core/domain/repositories/i_report_repository.hpp" // 引入 FormattedWeeklyReports 定义
#include "core/domain/types/report_format.hpp"
#include <filesystem>
#include <memory>
#include <string>
#include <vector>
namespace fs = std::filesystem;
class ReportFileManager;

class Exporter {
public:
  // [修改] 注入 Notifier
  Exporter(const fs::path &export_root_path,
           std::shared_ptr<core::interfaces::IFileSystem> fs,
           std::shared_ptr<core::interfaces::IUserNotifier> notifier);
  ~Exporter();

  void ExportSingleDayReport(const std::string &date,
                             const std::string &content,
                             ReportFormat format) const;
  void ExportSingleWeekReport(const std::string &week_name,
                              const std::string &content,
                              ReportFormat format) const;
  void ExportSingleMonthReport(const std::string &month,
                               const std::string &content,
                               ReportFormat format) const;
  void ExportSingleYearReport(const std::string &year,
                              const std::string &content,
                              ReportFormat format) const;
  void ExportSingleRecentReport(int days, const std::string &content,
                                ReportFormat format) const;
  void ExportSingleRangeReport(const std::string &start_date,
                               const std::string &end_date,
                               const std::string &content,
                               ReportFormat format) const;

  void ExportAllDailyReports(const FormattedGroupedReports &reports,
                             ReportFormat format) const;
  void ExportAllWeeklyReports(const FormattedWeeklyReports &reports,
                              ReportFormat format) const;
  void ExportAllMonthlyReports(const FormattedMonthlyReports &reports,
                               ReportFormat format) const;
  void ExportAllYearlyReports(const FormattedYearlyReports &reports,
                              ReportFormat format) const;
  void ExportAllRecentReports(const FormattedRecentReports &reports,
                              ReportFormat format) const;

private:
  void WriteReportToFile(const std::string &report_content,
                         const fs::path &output_path) const;
  std::unique_ptr<ReportFileManager> file_manager_;
  std::shared_ptr<core::interfaces::IFileSystem> fs_;
  std::shared_ptr<core::interfaces::IUserNotifier> notifier_; // [新增]
};
#endif
