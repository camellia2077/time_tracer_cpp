// core/infrastructure/reporting/exporter.cpp
#include "core/infrastructure/reporting/exporter.hpp"
#include "core/infrastructure/reporting/export_utils.hpp"
#include "core/infrastructure/reporting/report_file_manager.hpp"
#include <filesystem>


namespace fs = std::filesystem;

// [修改] 构造函数注入 Notifier
Exporter::Exporter(const fs::path& export_root_path, 
                   std::shared_ptr<core::interfaces::IFileSystem> fs,
                   std::shared_ptr<core::interfaces::IUserNotifier> notifier) 
    : fs_(std::move(fs)), notifier_(std::move(notifier)) {
    file_manager_ = std::make_unique<ReportFileManager>(export_root_path);
}

Exporter::~Exporter() = default;

void Exporter::write_report_to_file(const std::string& report_content, const fs::path& output_path) const {
    if (report_content.empty() || report_content.find("No time records") != std::string::npos) {
        return;
    }

    fs::path output_dir = output_path.parent_path();
    try {
        // 使用注入的 fs_ 接口
        fs_->create_directories(output_dir);
        fs_->write_content(output_path, report_content);
    } catch (const std::exception& e) {
        // [修改] 使用 notifier 报错
        notifier_->notify_error("Error: Failed to write report to " + output_path.string() + ": " + e.what());
    }
}

void Exporter::export_single_day_report(const std::string& date, const std::string& content, ReportFormat format) const {
    fs::path path = file_manager_->get_single_day_report_path(date, format);
    write_report_to_file(content, path);
    // [修改] 使用 notifier 提示成功
    notifier_->notify_success("Success: Report exported to " + fs::absolute(path).string());
}

void Exporter::export_single_month_report(const std::string& month, const std::string& content, ReportFormat format) const {
    fs::path path = file_manager_->get_single_month_report_path(month, format);
    write_report_to_file(content, path);
    // [修改] 使用 notifier 提示成功
    notifier_->notify_success("Success: Report exported to " + fs::absolute(path).string());
}

void Exporter::export_single_period_report(int days, const std::string& content, ReportFormat format) const {
    fs::path path = file_manager_->get_single_period_report_path(days, format);
    write_report_to_file(content, path);
    // [修改] 使用 notifier 提示成功
    notifier_->notify_success("Success: Report exported to " + fs::absolute(path).string());
}

void Exporter::export_all_daily_reports(const FormattedGroupedReports& reports, ReportFormat format) const {
    fs::path base_dir = file_manager_->get_all_daily_reports_base_dir(format);
    
    // [修改] 传递 notifier 给通用工具
    ExportUtils::execute_export_task("日报", base_dir, *notifier_, [&]() {
        int files_created = 0;
        for (const auto& year_pair : reports) {
            for (const auto& month_pair : year_pair.second) {
                for (const auto& day_report : month_pair.second) {
                    const std::string& date = day_report.first;
                    const std::string& content = day_report.second;
                    fs::path report_path = file_manager_->get_single_day_report_path(date, format);
                    write_report_to_file(content, report_path);
                    if (!content.empty()) files_created++;
                }
            }
        }
        return files_created;
    });
}

void Exporter::export_all_monthly_reports(const FormattedMonthlyReports& reports, ReportFormat format) const {
    fs::path base_dir = file_manager_->get_all_monthly_reports_base_dir(format);

    // [修改] 传递 notifier 给通用工具
    ExportUtils::execute_export_task("月报", base_dir, *notifier_, [&]() {
        int files_created = 0;
        for (const auto& year_pair : reports) {
            int year = year_pair.first;
            for (const auto& month_pair : year_pair.second) {
                int month = month_pair.first;
                const std::string& content = month_pair.second;
                std::string month_str = std::to_string(year) + (month < 10 ? "0" : "") + std::to_string(month);
                
                fs::path report_path = file_manager_->get_single_month_report_path(month_str, format);
                write_report_to_file(content, report_path);
                if (!content.empty()) files_created++;
            }
        }
        return files_created;
    });
}

void Exporter::export_all_period_reports(const FormattedPeriodReports& reports, ReportFormat format) const {
    fs::path base_dir = file_manager_->get_all_period_reports_base_dir(format);
    
    // [修改] 传递 notifier 给通用工具
    ExportUtils::execute_export_task("周期报告", base_dir, *notifier_, [&]() {
        int files_created = 0;
        for (const auto& report_pair : reports) {
            int days = report_pair.first;
            const std::string& content = report_pair.second;
            fs::path report_path = file_manager_->get_single_period_report_path(days, format);
            write_report_to_file(content, report_path);
            if (!content.empty()) files_created++;
        }
        return files_created;
    });
}