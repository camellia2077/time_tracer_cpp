// application/service/report_handler.cpp
#include "application/service/report_handler.hpp"
#include "application/service/report_generator.hpp"
#include "core/infrastructure/reporting/exporter.hpp"
#include <filesystem>
#include <format> // [ä¿®å¤] æ·»å æ­¤å¤´æä»¶ä»¥æ¯æ?std::format
#include <iostream>
#include <sstream>

#include "reports/domain/model/daily_report_data.hpp"
#include "reports/domain/model/range_report_data.hpp"
#include "reports/shared/factories/generic_formatter_factory.hpp"

namespace fs = std::filesystem;

static void ensure_formatters_registered(const std::string &root_dir_str) {
  static bool registered = false;
  if (registered)
    return;

  fs::path root_dir(root_dir_str);
  fs::path plugin_dir = root_dir / "plugins";

  // ç¡®ä¿æ¯ç»å¯¹è·¯å¾?
  if (plugin_dir.is_relative()) {
    plugin_dir = fs::absolute(plugin_dir);
  }

  // å¦ææä»¶ç®å½ä¸å­å¨ï¼æå°è­¦åï¼æ¹ä¾¿è°è¯ï¼
  if (!fs::exists(plugin_dir)) {
    std::cerr << "[ReportHandler] Warning: Plugins directory not found at "
              << plugin_dir << std::endl;
  }

  // --- æ³¨åæ¥æ¥ (DailyReportData)
  // çæ ¼å¼åæä»¶
  // --- Markdown
  GenericFormatterFactory<DailyReportData>::register_dll_formatter(
      ReportFormat::Markdown, (plugin_dir / "DayMdFormatter").string());
  // LaTeX
  GenericFormatterFactory<DailyReportData>::register_dll_formatter(
      ReportFormat::LaTeX, (plugin_dir / "DayTexFormatter").string());
  // Typst
  GenericFormatterFactory<DailyReportData>::register_dll_formatter(
      ReportFormat::Typ, (plugin_dir / "DayTypFormatter").string());

  // --- æ³¨åèå´æ¥è¡¨ (RangeReportData)
  // çæ ¼å¼åæä»¶
  // --- Markdown
  GenericFormatterFactory<RangeReportData>::register_dll_formatter(
      ReportFormat::Markdown, (plugin_dir / "RangeMdFormatter").string());
  // LaTeX
  GenericFormatterFactory<RangeReportData>::register_dll_formatter(
      ReportFormat::LaTeX, (plugin_dir / "RangeTexFormatter").string());
  // Typst
  GenericFormatterFactory<RangeReportData>::register_dll_formatter(
      ReportFormat::Typ, (plugin_dir / "RangeTypFormatter").string());

  registered = true;
}

ReportHandler::ReportHandler(std::unique_ptr<ReportGenerator> generator,
                             std::unique_ptr<Exporter> exporter,
                             const std::string &app_root_dir)
    : generator_(std::move(generator)), exporter_(std::move(exporter)) {

  // ä¼ å¥ exe æå¨ç®å½?
  ensure_formatters_registered(app_root_dir);
}

ReportHandler::~ReportHandler() = default;

std::string ReportHandler::RunDailyQuery(const std::string &date,
                                         ReportFormat format) {
  return generator_->GenerateDailyReport(date, format);
}

std::string ReportHandler::RunMonthlyQuery(const std::string &month,
                                           ReportFormat format) {
  return generator_->GenerateMonthlyReport(month, format);
}

// [ä¿®å¤] å®ç°äºç¼ºå¤±ç run_weekly_query
// å½æ°ï¼è§£å³é¾æ¥éè¯?
std::string ReportHandler::RunWeeklyQuery(int year, int week,
                                          ReportFormat format) {
  return generator_->GenerateWeeklyReport(year, week, format);
}

std::string ReportHandler::RunPeriodQuery(int days, ReportFormat format) {
  return generator_->GeneratePeriodReport(days, format);
}

std::string ReportHandler::RunPeriodQueries(const std::vector<int> &days_list,
                                            ReportFormat format) {
  std::ostringstream oss;
  for (size_t i = 0; i < days_list.size(); ++i) {
    if (i > 0) {
      oss << "\n" << std::string(40, '-') << "\n";
    }
    try {
      oss << RunPeriodQuery(days_list[i], format);
    } catch (const std::exception &e) {
      oss << "Error querying period " << days_list[i] << " days: " << e.what();
    }
  }
  return oss.str();
}

void ReportHandler::RunExportSingleWeekReport(int year, int week,
                                              ReportFormat format) {
  auto content = generator_->GenerateWeeklyReport(year, week, format);
  // æé å¨æ¥åç§°ï¼ä¾å¦ "2025-W04"
  std::string week_name = std::format("{}-W{:02d}", year, week);
  exporter_->ExportSingleWeekReport(week_name, content, format);
}

void ReportHandler::RunExportSingleDayReport(const std::string &date,
                                             ReportFormat format) {
  auto content = generator_->GenerateDailyReport(date, format);
  exporter_->ExportSingleDayReport(date, content, format);
}

void ReportHandler::RunExportSingleMonthReport(const std::string &month,
                                               ReportFormat format) {
  auto content = generator_->GenerateMonthlyReport(month, format);
  exporter_->ExportSingleMonthReport(month, content, format);
}

void ReportHandler::RunExportSinglePeriodReport(int days, ReportFormat format) {
  auto content = generator_->GeneratePeriodReport(days, format);
  exporter_->ExportSinglePeriodReport(days, content, format);
}

void ReportHandler::RunExportAllDailyReportsQuery(ReportFormat format) {
  auto reports = generator_->GenerateAllDailyReports(format);
  exporter_->ExportAllDailyReports(reports, format);
}

void ReportHandler::RunExportAllWeeklyReportsQuery(ReportFormat format) {
  auto reports = generator_->GenerateAllWeeklyReports(format);
  exporter_->ExportAllWeeklyReports(reports, format);
}

void ReportHandler::RunExportAllMonthlyReportsQuery(ReportFormat format) {
  auto reports = generator_->GenerateAllMonthlyReports(format);
  exporter_->ExportAllMonthlyReports(reports, format);
}

void ReportHandler::RunExportAllPeriodReportsQuery(
    const std::vector<int> &days_list, ReportFormat format) {
  auto reports = generator_->GenerateAllPeriodReports(days_list, format);
  exporter_->ExportAllPeriodReports(reports, format);
}
