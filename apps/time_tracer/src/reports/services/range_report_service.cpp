// reports/services/range_report_service.cpp
#include "reports/services/range_report_service.hpp"
#include "reports/data/cache/project_name_cache.hpp"
#include "reports/data/utils/project_tree_builder.hpp"
#include "reports/shared/factories/generic_formatter_factory.hpp"
#include <iostream>

// 辅助函数：解析 "YYYY-MM"
static std::pair<int, int> parse_year_month(const std::string& ym) {
    if (ym.size() >= 7 && ym[4] == '-') {
        try {
            int y = std::stoi(ym.substr(0, 4));
            int m = std::stoi(ym.substr(5, 2));
            return {y, m};
        } catch (...) {}
    }
    return {0, 0};
}

RangeReportService::RangeReportService(IReportRepository& repo, const AppConfig& config)
    : repo_(repo), app_config_(config) {}

RangeReportData RangeReportService::build_data_for_range(const RangeRequest& request) {
    RangeReportData data;
    data.report_name = request.name;
    data.start_date = request.start_date;
    data.end_date = request.end_date;
    data.covered_days = request.covered_days;

    // 1. 获取聚合数据
    data.project_stats = repo_.get_aggregated_project_stats(request.start_date, request.end_date);

    // 2. 获取实际活跃天数
    data.actual_active_days = repo_.get_actual_active_days(request.start_date, request.end_date);

    // 3. 计算总时长
    for (const auto& p : data.project_stats) {
        data.total_duration += p.second;
    }

    // 4. 构建树 (仅当有数据时)
    if (data.total_duration > 0) {
        auto& name_cache = ProjectNameCache::instance();
        build_project_tree_from_ids(data.project_tree, data.project_stats, name_cache);
    }

    return data;
}

std::string RangeReportService::generate_report(const RangeRequest& request, ReportFormat format) {
    RangeReportData data = build_data_for_range(request);
    
    auto formatter = GenericFormatterFactory<RangeReportData>::create(format, app_config_);
    return formatter->format_report(data);
}

std::map<std::string, std::string> RangeReportService::generate_batch(
    const std::vector<RangeRequest>& requests, 
    ReportFormat format) 
{
    std::map<std::string, std::string> results;
    // 这里如果 formatter 初始化开销大，可以复用 formatter 实例，
    // 但 GenericFormatterFactory 目前设计是 create 返回 unique_ptr。
    // 考虑到 DLL 加载可能有缓存，直接循环调用 generate_report 简单可靠。
    for (const auto& req : requests) {
        results[req.name] = generate_report(req, format);
    }
    return results;
}

std::map<int, std::map<int, std::string>> RangeReportService::generate_all_monthly_history(ReportFormat format) {
    std::map<int, std::map<int, std::string>> reports;
    auto& name_cache = ProjectNameCache::instance();

    // 1. 批量获取数据 (复用 MonthlyReportService 的优化逻辑)
    auto all_project_stats = repo_.get_all_months_project_stats();
    auto all_active_days = repo_.get_all_months_active_days();

    auto formatter = GenericFormatterFactory<RangeReportData>::create(format, app_config_);

    // 2. 内存处理
    for (auto& [ym, stats] : all_project_stats) {
        RangeReportData data;
        data.report_name = ym;
        // 构造粗略的起止日期用于显示，虽然核心统计已经查出来了
        data.start_date = ym + "-01";
        data.end_date = ym + "-31"; // 只是展示用，不影响统计
        data.covered_days = 30;     // 估算值，或者可以通过日期库计算准确天数
        data.project_stats = stats;
        data.actual_active_days = all_active_days[ym];

        for(auto& p : stats) data.total_duration += p.second;

        if (data.total_duration > 0) {
            build_project_tree_from_ids(data.project_tree, data.project_stats, name_cache);
            
            std::string content = formatter->format_report(data);

            auto [year, month] = parse_year_month(ym);
            if (year > 0 && month > 0) {
                reports[year][month] = content;
            }
        }
    }

    return reports;
}