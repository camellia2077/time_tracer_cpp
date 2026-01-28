// core/infrastructure/reporting/exporter.cpp
#include "core/infrastructure/reporting/exporter.hpp"
#include "core/infrastructure/reporting/export_utils.hpp"
#include "core/infrastructure/reporting/report_file_manager.hpp"
#include <filesystem>

namespace fs = std::filesystem;

// [修改] 构造函数注入 Notifier
Exporter::Exporter(const fs::path &export_root_path,
                   std::shared_ptr<core::interfaces::IFileSystem> fs,
                   std::shared_ptr<core::interfaces::IUserNotifier> notifier)
    : fs_(std::move(fs)), notifier_(std::move(notifier)) {
  file_manager_ = std::make_unique<ReportFileManager>(export_root_path);
}

Exporter::~Exporter() = default;

void Exporter::WriteReportToFile(const std::string &report_content,
                                 const fs::path &output_path) const {
  if (report_content.empty() ||
      report_content.find("No time records") != std::string::npos) {
    return;
  }

  fs::path output_dir = output_path.parent_path();
  try {
    // 使用注入的 fs_ 接口
    fs_->CreateDirectories(output_dir);
    fs_->WriteContent(output_path, report_content);
  } catch (const std::exception &e) {
    // [修改] 使用 notifier 报错
    notifier_->NotifyError("Error: Failed to write report to " +
                           output_path.string() + ": " + e.what());
  }
}

void Exporter::ExportSingleDayReport(const std::string &date,
                                     const std::string &content,
                                     ReportFormat format) const {
  fs::path path = file_manager_->GetSingleDayReportPath(date, format);
  WriteReportToFile(content, path);
  // [修改] 使用 notifier 提示成功
  notifier_->NotifySuccess("Success: Report exported to " +
                           fs::absolute(path).string());
}
void Exporter::ExportSingleWeekReport(const std::string &week_name,
                                      const std::string &content,
                                      ReportFormat format) const {
  fs::path path = file_manager_->GetSingleWeekReportPath(week_name, format);
  WriteReportToFile(content, path);
  notifier_->NotifySuccess("Success: Weekly report exported to " +
                           fs::absolute(path).string());
}

void Exporter::ExportSingleMonthReport(const std::string &month,
                                       const std::string &content,
                                       ReportFormat format) const {
  fs::path path = file_manager_->GetSingleMonthReportPath(month, format);
  WriteReportToFile(content, path);
  // [修改] 使用 notifier 提示成功
  notifier_->NotifySuccess("Success: Report exported to " +
                           fs::absolute(path).string());
}

void Exporter::ExportSinglePeriodReport(int days, const std::string &content,
                                        ReportFormat format) const {
  fs::path path = file_manager_->GetSinglePeriodReportPath(days, format);
  WriteReportToFile(content, path);
  // [修改] 使用 notifier 提示成功
  notifier_->NotifySuccess("Success: Report exported to " +
                           fs::absolute(path).string());
}

void Exporter::ExportAllDailyReports(const FormattedGroupedReports &reports,
                                     ReportFormat format) const {
  fs::path base_dir = file_manager_->GetAllDailyReportsBaseDir(format);

  // [修改] 传递 notifier 给通用工具
  ExportUtils::ExecuteExportTask("日报", base_dir, *notifier_, [&]() {
    int files_created = 0;
    for (const auto &year_pair : reports) {
      for (const auto &month_pair : year_pair.second) {
        for (const auto &day_report : month_pair.second) {
          const std::string &date = day_report.first;
          const std::string &content = day_report.second;
          fs::path report_path =
              file_manager_->GetSingleDayReportPath(date, format);
          WriteReportToFile(content, report_path);
          if (!content.empty())
            files_created++;
        }
      }
    }
    return files_created;
  });
}

// [修复] 实现 ExportAllWeeklyReports 方法
void Exporter::ExportAllWeeklyReports(const FormattedWeeklyReports &reports,
                                      ReportFormat format) const {
  fs::path base_dir = file_manager_->GetAllWeeklyReportsBaseDir(format);

  ExportUtils::ExecuteExportTask("周报", base_dir, *notifier_, [&]() {
    int files_created = 0;
    for (const auto &year_pair : reports) {
      int year = year_pair.first;
      // 建议按年份分文件夹: weekly/2025/
      fs::path year_dir = base_dir / std::to_string(year);

      for (const auto &week_pair : year_pair.second) {
        int week = week_pair.first;
        const std::string &content = week_pair.second;

        // 文件名: 2025-W01.md
        std::string filename = std::format("{}-W{:02d}", year, week);

        // 获取后缀并构造完整路径
        auto details = ExportUtils::GetReportFormatDetails(format).value();
        fs::path report_path = year_dir / (filename + details.extension_);

        WriteReportToFile(content, report_path);
        if (!content.empty())
          files_created++;
      }
    }
    return files_created;
  });
}

void Exporter::ExportAllMonthlyReports(const FormattedMonthlyReports &reports,
                                       ReportFormat format) const {
  fs::path base_dir = file_manager_->GetAllMonthlyReportsBaseDir(format);

  // [修改] 传递 notifier 给通用工具
  ExportUtils::ExecuteExportTask("月报", base_dir, *notifier_, [&]() {
    int files_created = 0;
    for (const auto &year_pair : reports) {
      int year = year_pair.first;
      for (const auto &month_pair : year_pair.second) {
        int month = month_pair.first;
        const std::string &content = month_pair.second;
        std::string month_str = std::to_string(year) + (month < 10 ? "0" : "") +
                                std::to_string(month);

        fs::path report_path =
            file_manager_->GetSingleMonthReportPath(month_str, format);
        WriteReportToFile(content, report_path);
        if (!content.empty())
          files_created++;
      }
    }
    return files_created;
  });
}

void Exporter::ExportAllPeriodReports(const FormattedPeriodReports &reports,
                                      ReportFormat format) const {
  fs::path base_dir = file_manager_->GetAllPeriodReportsBaseDir(format);

  // [修改] 传递 notifier 给通用工具
  ExportUtils::ExecuteExportTask("周期报告", base_dir, *notifier_, [&]() {
    int files_created = 0;
    for (const auto &report_pair : reports) {
      int days = report_pair.first;
      const std::string &content = report_pair.second;
      fs::path report_path =
          file_manager_->GetSinglePeriodReportPath(days, format);
      WriteReportToFile(content, report_path);
      if (!content.empty())
        files_created++;
    }
    return files_created;
  });
}
