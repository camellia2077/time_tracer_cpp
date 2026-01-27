// reports/services/daily_report_service.cpp
#include "reports/services/daily_report_service.hpp"
#include "reports/core/factories/generic_formatter_factory.hpp"
#include "reports/data/queriers/daily/day_querier.hpp"
#include "reports/data/utils/project_tree_builder.hpp"
#include "reports/data/cache/project_name_cache.hpp"
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
// [移除] 移除了 fstream, sstream, filesystem，因为不再进行 IO 操作

// 辅助函数保持不变
static std::string join_path_parts(const std::vector<std::string>& parts) {
    if (parts.empty()) return "";
    std::string path = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) {
        path += "_" + parts[i];
    }
    return path;
}

static std::pair<int, int> parse_year_month(const std::string& date) {
    if (date.size() >= 7) {
        try {
            int y = std::stoi(date.substr(0, 4));
            int m = std::stoi(date.substr(5, 2));
            return {y, m};
        } catch (...) {}
    }
    return {0, 0};
}

// [修改] 构造函数
DailyReportService::DailyReportService(IReportRepository& repo, const GlobalReportConfig& config)
    : repo_(repo), config_(config) {}

// [新增] 辅助函数实现
const DailyReportConfig& DailyReportService::get_config_by_format(ReportFormat format) const {
    switch (format) {
        case ReportFormat::Markdown: return config_.markdown.day;
        case ReportFormat::LaTeX:    return config_.latex.day;
        case ReportFormat::Typ:      return config_.typst.day;
        default: throw std::runtime_error("Unsupported format in DailyReportService");
    }
}

// [修改] 生成报表
std::string DailyReportService::generate_report(const std::string& date, ReportFormat format) {
    // 1. 获取数据
    DayQuerier querier(repo_, date);
    DailyReportData data = querier.fetch_data();

    // 2. 获取配置对象
    const auto& cfg = get_config_by_format(format);

    // 3. 获取格式化器 (传递 Struct)
    // 注意：假设 Factory 已经适配为接收 (ReportFormat, const DailyReportConfig&)
    auto formatter = GenericFormatterFactory<DailyReportData>::create(format, cfg);

    // 4. 生成
    return formatter->format_report(data);
}

// [修改] 批量生成
FormattedGroupedReports DailyReportService::generate_all_reports(ReportFormat format) {
    FormattedGroupedReports grouped_reports;

    auto& name_cache = ProjectNameCache::instance();

    std::map<std::string, DailyReportData> data_map = repo_.get_all_days_metadata();
    auto all_records = repo_.get_all_time_records_with_date();

    // [修改] 预获取配置对象
    const auto& cfg = get_config_by_format(format);

    std::map<std::string, std::map<long long, long long>> temp_proj_agg;

    for (auto& [date, record] : all_records) {
        DailyReportData& day_data = data_map[date];
        if (day_data.date.empty()) day_data.date = date;

        long long pid = 0;
        try { pid = std::stoll(record.project_path); } catch (...) {}

        temp_proj_agg[date][pid] += record.duration_seconds;

        std::vector<std::string> parts = name_cache.get_path_parts(pid);
        record.project_path = join_path_parts(parts);

        day_data.detailed_records.push_back(record);
        day_data.total_duration += record.duration_seconds;
    }
    
    for(auto& [date, agg] : temp_proj_agg) {
        auto& day_data = data_map[date];
        for(auto& kv : agg) {
            day_data.project_stats.push_back(kv);
        }
    }

    // [修改] 创建格式化器 (传递 Struct)
    auto formatter = GenericFormatterFactory<DailyReportData>::create(format, cfg);

    for (auto& [date, data] : data_map) {
        if (data.total_duration > 0) {
            build_project_tree_from_ids(data.project_tree, data.project_stats, name_cache);
            
            std::string formatted_report = formatter->format_report(data);
            
            auto [year, month] = parse_year_month(date);
            grouped_reports[year][month].push_back({date, formatted_report});
        }
    }

    return grouped_reports;
}