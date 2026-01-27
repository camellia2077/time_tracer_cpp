// core/infrastructure/persistence/sqlite_report_repository_adapter.cpp
#include "core/infrastructure/persistence/sqlite_report_repository_adapter.hpp"
#include "reports/shared/utils/format/time_format.hpp" 
#include <stdexcept>
#include <format> 
#include <iostream>

#include "reports/services/range_report_service.hpp"
#include "reports/services/daily_report_service.hpp"

// [修改] 必须包含新的底层仓库头文件
#include "reports/data/repositories/sqlite_report_data_repository.hpp" 

namespace infrastructure::persistence {

SqliteReportRepositoryAdapter::SqliteReportRepositoryAdapter(sqlite3* db, const GlobalReportConfig& config) {
    // [修复] 这里创建的是底层的 SqliteReportDataRepository，而不是 Adapter
    data_repo_ = std::make_unique<::SqliteReportDataRepository>(db);
    
    // 初始化 Service，把底层 Repo 传给它们
    daily_service_ = std::make_unique<DailyReportService>(*data_repo_, config);
    range_service_ = std::make_unique<RangeReportService>(*data_repo_, config);
}

SqliteReportRepositoryAdapter::~SqliteReportRepositoryAdapter() = default;

std::string SqliteReportRepositoryAdapter::get_daily_report(const std::string& date, ReportFormat format) {
    return daily_service_->generate_report(date, format);
}

std::string SqliteReportRepositoryAdapter::get_monthly_report(const std::string& month, ReportFormat format) {
    RangeRequest req;
    req.name = month;
    req.start_date = month + "-01";
    req.end_date = month + "-31"; // 简化的逻辑，Service内部如果不依赖此日期计算ActiveDays则无妨，否则建议用工具计算准确月底
    req.covered_days = 30;
    req.type = RangeType::Month; // [新增] 指定类型

    return range_service_->generate_report(req, format);
}

std::string SqliteReportRepositoryAdapter::get_weekly_report(int year, int week, ReportFormat format) {
    auto [start_date, end_date] = get_week_date_range(year, week);

    RangeRequest req;
    req.name = std::format("{}-W{:02d}", year, week);
    req.start_date = start_date;
    req.end_date = end_date;
    req.covered_days = 7;
    req.type = RangeType::Week; // [新增] 指定类型

    return range_service_->generate_report(req, format);
}

std::string SqliteReportRepositoryAdapter::get_period_report(int days, ReportFormat format) {
    // 1. 使用新工具计算日期范围
    auto [start_date, end_date] = get_period_dates(days);

    // 2. 构造请求
    RangeRequest req;
    req.name = "Last " + std::to_string(days) + " Days";
    req.start_date = start_date;
    req.end_date = end_date;
    req.covered_days = days;
    req.type = RangeType::Period; // 明确指定为 Period 类型
    
    // 3. 生成报告
    return range_service_->generate_report(req, format);
}

FormattedGroupedReports SqliteReportRepositoryAdapter::get_all_daily_reports(ReportFormat format) {
    // [修复] 方法名更正为 generate_all_reports，匹配 DailyReportService 的定义
    return daily_service_->generate_all_reports(format);
}

FormattedMonthlyReports SqliteReportRepositoryAdapter::get_all_monthly_reports(ReportFormat format) {
    return range_service_->generate_all_monthly_history(format);
}

FormattedPeriodReports SqliteReportRepositoryAdapter::get_all_period_reports(const std::vector<int>& days_list, ReportFormat format) {
    std::vector<RangeRequest> reqs;
    
    // 1. 批量构造请求
    for(int days : days_list) {
        auto [start, end] = get_period_dates(days);
        
        RangeRequest req;
        // 使用天数作为临时 name，方便后续映射回 map<int, string>
        req.name = std::to_string(days); 
        req.start_date = start;
        req.end_date = end;
        req.covered_days = days;
        req.type = RangeType::Period;
        
        reqs.push_back(req);
    }

    // 2. 批量生成 (generate_batch 返回 map<string, string>)
    auto results_map = range_service_->generate_batch(reqs, format);

    // 3. 转换结果格式 map<string, string> -> map<int, string>
    FormattedPeriodReports final_reports;
    for(const auto& [days_str, content] : results_map) {
        try {
            int days = std::stoi(days_str);
            final_reports[days] = content;
        } catch (...) {
            continue;
        }
    }
    
    return final_reports;
}

FormattedWeeklyReports SqliteReportRepositoryAdapter::get_all_weekly_reports(ReportFormat format) {
    return range_service_->generate_all_weekly_history(format);
}

} // namespace