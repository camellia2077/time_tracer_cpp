// application/service/report_handler.cpp
#include "application/service/report_handler.hpp"
#include "application/service/report_generator.hpp"
#include "core/infrastructure/reporting/exporter.hpp"
#include <iostream>
#include <sstream>
#include <filesystem>
#include <format> // [修复] 添加此头文件以支�?std::format

#include "reports/core/factories/generic_formatter_factory.hpp"
#include "reports/data/model/daily_report_data.hpp"
#include "reports/data/model/range_report_data.hpp"

namespace fs = std::filesystem;

static void ensure_formatters_registered(const std::string& root_dir_str) {
    static bool registered = false;
    if (registered) return;

    fs::path root_dir(root_dir_str);
    fs::path plugin_dir = root_dir / "plugins";

    // 确保是绝对路�?
    if (plugin_dir.is_relative()) {
        plugin_dir = fs::absolute(plugin_dir);
    }
    
    // 如果插件目录不存在，打印警告（方便调试）
    if (!fs::exists(plugin_dir)) {
        std::cerr << "[ReportHandler] Warning: Plugins directory not found at " << plugin_dir << std::endl;
    }

    // --- 注册日报 (DailyReportData) 的格式化插件 ---
    // Markdown
    GenericFormatterFactory<DailyReportData>::register_dll_formatter(ReportFormat::Markdown, (plugin_dir / "DayMdFormatter").string());
    // LaTeX
    GenericFormatterFactory<DailyReportData>::register_dll_formatter(ReportFormat::LaTeX, (plugin_dir / "DayTexFormatter").string());
    // Typst
    GenericFormatterFactory<DailyReportData>::register_dll_formatter(ReportFormat::Typ, (plugin_dir / "DayTypFormatter").string());

    // --- 注册范围报表 (RangeReportData) 的格式化插件 ---
    // Markdown
    GenericFormatterFactory<RangeReportData>::register_dll_formatter(ReportFormat::Markdown, (plugin_dir / "RangeMdFormatter").string());
    // LaTeX
    GenericFormatterFactory<RangeReportData>::register_dll_formatter(ReportFormat::LaTeX, (plugin_dir / "RangeTexFormatter").string());
    // Typst
    GenericFormatterFactory<RangeReportData>::register_dll_formatter(ReportFormat::Typ, (plugin_dir / "RangeTypFormatter").string());

    registered = true;
}

ReportHandler::ReportHandler(std::unique_ptr<ReportGenerator> generator, 
                             std::unique_ptr<Exporter> exporter,
                             const std::string& app_root_dir)
    : generator_(std::move(generator)), exporter_(std::move(exporter)) {
    
    // 传入 exe 所在目�?
    ensure_formatters_registered(app_root_dir);
}

ReportHandler::~ReportHandler() = default;

std::string ReportHandler::run_daily_query(const std::string& date, ReportFormat format) {
    return generator_->generate_daily_report(date, format);
}

std::string ReportHandler::run_monthly_query(const std::string& month, ReportFormat format) {
    return generator_->generate_monthly_report(month, format);
}

// [修复] 实现了缺失的 run_weekly_query 函数，解决链接错�?
std::string ReportHandler::run_weekly_query(int year, int week, ReportFormat format) {
    return generator_->generate_weekly_report(year, week, format);
}

std::string ReportHandler::run_period_query(int days, ReportFormat format) {
    return generator_->generate_period_report(days, format);
}

std::string ReportHandler::run_period_queries(const std::vector<int>& days_list, ReportFormat format) {
    std::ostringstream oss;
    for (size_t i = 0; i < days_list.size(); ++i) {
        if (i > 0) {
            oss << "\n" << std::string(40, '-') << "\n";
        }
        try {
            oss << run_period_query(days_list[i], format);
        } catch (const std::exception& e) {
            oss << "Error querying period " << days_list[i] << " days: " << e.what();
        }
    }
    return oss.str();
}

void ReportHandler::run_export_single_week_report(int year, int week, ReportFormat format) {
    auto content = generator_->generate_weekly_report(year, week, format);
    // 构造周报名称，例如 "2025-W04"
    std::string week_name = std::format("{}-W{:02d}", year, week);
    exporter_->export_single_week_report(week_name, content, format);
}

void ReportHandler::run_export_single_day_report(const std::string& date, ReportFormat format) {
    auto content = generator_->generate_daily_report(date, format);
    exporter_->export_single_day_report(date, content, format);
}

void ReportHandler::run_export_single_month_report(const std::string& month, ReportFormat format) {
    auto content = generator_->generate_monthly_report(month, format);
    exporter_->export_single_month_report(month, content, format);
}

void ReportHandler::run_export_single_period_report(int days, ReportFormat format) {
    auto content = generator_->generate_period_report(days, format);
    exporter_->export_single_period_report(days, content, format);
}

void ReportHandler::run_export_all_daily_reports_query(ReportFormat format) {
    auto reports = generator_->generate_all_daily_reports(format);
    exporter_->export_all_daily_reports(reports, format);
}

void ReportHandler::run_export_all_weekly_reports_query(ReportFormat format) {
    auto reports = generator_->generate_all_weekly_reports(format);
    exporter_->export_all_weekly_reports(reports, format);
}

void ReportHandler::run_export_all_monthly_reports_query(ReportFormat format) {
    auto reports = generator_->generate_all_monthly_reports(format);
    exporter_->export_all_monthly_reports(reports, format);
}

void ReportHandler::run_export_all_period_reports_query(const std::vector<int>& days_list, ReportFormat format) {
    auto reports = generator_->generate_all_period_reports(days_list, format);
    exporter_->export_all_period_reports(reports, format);
}