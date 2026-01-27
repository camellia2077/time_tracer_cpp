// core/infrastructure/reporting/exporter.hpp
#ifndef CORE_INFRASTRUCTURE_REPORTING_EXPORTER_HPP_
#define CORE_INFRASTRUCTURE_REPORTING_EXPORTER_HPP_

#include <string>
#include <vector>
#include <filesystem>
#include <memory>
#include "reports/core/types/report_format.hpp"
#include "reports/data/model/query_data_structs.hpp"
#include "application/interfaces/i_file_system.hpp"
#include "application/interfaces/i_user_notifier.hpp" // [新增]
#include "core/domain/repositories/i_report_repository.hpp" // 引入 FormattedWeeklyReports 定义
namespace fs = std::filesystem;
class ReportFileManager;

class Exporter {
public:
    // [修改] 注入 Notifier
    Exporter(const fs::path& export_root_path, 
             std::shared_ptr<core::interfaces::IFileSystem> fs,
             std::shared_ptr<core::interfaces::IUserNotifier> notifier);
    ~Exporter();

    void export_single_day_report(const std::string& date, const std::string& content, ReportFormat format) const;
    void export_single_week_report(const std::string& week_name, const std::string& content, ReportFormat format) const;
    void export_single_month_report(const std::string& month, const std::string& content, ReportFormat format) const;
    void export_single_period_report(int days, const std::string& content, ReportFormat format) const;
    

    void export_all_daily_reports(const FormattedGroupedReports& reports, ReportFormat format) const;
    void export_all_weekly_reports(const FormattedWeeklyReports& reports, ReportFormat format) const;
    void export_all_monthly_reports(const FormattedMonthlyReports& reports, ReportFormat format) const;
    void export_all_period_reports(const FormattedPeriodReports& reports, ReportFormat format) const;

private:
    void write_report_to_file(const std::string& report_content, const fs::path& output_path) const;
    std::unique_ptr<ReportFileManager> file_manager_;
    std::shared_ptr<core::interfaces::IFileSystem> fs_;
    std::shared_ptr<core::interfaces::IUserNotifier> notifier_; // [新增]
};
#endif