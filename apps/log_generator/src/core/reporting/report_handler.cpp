// core/reporting/report_handler.cpp
#include "core/reporting/report_handler.hpp"
#include "core/reporting/generator/report_generator.hpp" // 这里有完整定义
#include "core/reporting/export/exporter.hpp"            // 这里有完整定义
#include <iostream>
#include <sstream>

ReportHandler::~ReportHandler() = default;

ReportHandler::ReportHandler(std::unique_ptr<ReportGenerator> generator, std::unique_ptr<Exporter> exporter)
    : generator_(std::move(generator)), exporter_(std::move(exporter)) {}

std::string ReportHandler::run_daily_query(const std::string& date, ReportFormat format) {
    return generator_->generate_daily_report(date, format);
}

std::string ReportHandler::run_monthly_query(const std::string& month, ReportFormat format) {
    return generator_->generate_monthly_report(month, format);
}

std::string ReportHandler::run_period_query(int days, ReportFormat format) {
    return generator_->generate_period_report(days, format);
}

// [新增] 批量查询逻辑下沉至 Core
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

void ReportHandler::run_export_single_day_report(const std::string& date, ReportFormat format) {
    auto content = generator_->generate_daily_report(date, format);
    // [修复] 调用 export_single_day_report
    exporter_->export_single_day_report(date, content, format);
}

void ReportHandler::run_export_single_month_report(const std::string& month, ReportFormat format) {
    auto content = generator_->generate_monthly_report(month, format);
    // [修复] 调用 export_single_month_report
    exporter_->export_single_month_report(month, content, format);
}

void ReportHandler::run_export_single_period_report(int days, ReportFormat format) {
    auto content = generator_->generate_period_report(days, format);
    // [修复] 调用 export_single_period_report
    exporter_->export_single_period_report(days, content, format);
}

void ReportHandler::run_export_all_daily_reports_query(ReportFormat format) {
    // [修复] 获取整个集合
    auto reports = generator_->generate_all_daily_reports(format);
    // [修复] 直接传递集合给 Exporter
    exporter_->export_all_daily_reports(reports, format);
}

void ReportHandler::run_export_all_monthly_reports_query(ReportFormat format) {
    // [修复] 获取整个集合
    auto reports = generator_->generate_all_monthly_reports(format);
    // [修复] 直接传递集合
    exporter_->export_all_monthly_reports(reports, format);
}

void ReportHandler::run_export_all_period_reports_query(const std::vector<int>& days_list, ReportFormat format) {
    // 针对 Period，我们也可以优化为使用 Generator 和 Exporter 的批量接口
    auto reports = generator_->generate_all_period_reports(days_list, format);
    exporter_->export_all_period_reports(reports, format);
}