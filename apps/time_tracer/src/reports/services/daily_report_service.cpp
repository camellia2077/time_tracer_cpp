// reports/services/daily_report_service.cpp
#include "reports/services/daily_report_service.hpp"
#include "reports/shared/factories/generic_formatter_factory.hpp"
#include "reports/data/queriers/daily/day_querier.hpp"
#include "reports/data/utils/project_tree_builder.hpp"
#include "reports/data/cache/project_name_cache.hpp"
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>

// 辅助函数：将路径部分拼接为字符串
static std::string join_path_parts(const std::vector<std::string>& parts) {
    if (parts.empty()) return "";
    std::string path = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) {
        path += "_" + parts[i];
    }
    return path;
}

// [新增实现]
std::string DailyReportService::generate_report(const std::string& date, ReportFormat format) {
    // 1. 获取数据
    DayQuerier querier(repo_, date);
    DailyReportData data = querier.fetch_data();

    // 2. 获取格式化器
    auto formatter = GenericFormatterFactory<DailyReportData>::create(format, app_config_);

    // 3. 生成
    return formatter->format_report(data);
}

// 辅助函数：解析日期中的年月日
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

DailyReportService::DailyReportService(IReportRepository& repo, const AppConfig& config)
    : repo_(repo), app_config_(config) {}

FormattedGroupedReports DailyReportService::generate_all_reports(ReportFormat format) {
    FormattedGroupedReports grouped_reports;

    // 1. 准备项目名称缓存
    auto& name_cache = ProjectNameCache::instance();

    // 2. 从 Repository 批量获取数据
    std::map<std::string, DailyReportData> data_map = repo_.get_all_days_metadata();
    auto all_records = repo_.get_all_time_records_with_date();

    // 3. 处理记录：ID 转 Path，并归类到 data_map
    // 内存中聚合 Project Stats 以便构建树
    std::map<std::string, std::map<long long, long long>> temp_proj_agg;

    for (auto& [date, record] : all_records) {
        // 如果该日期不在 metadata 中，自动创建条目
        DailyReportData& day_data = data_map[date];
        if (day_data.date.empty()) day_data.date = date;

        long long pid = 0;
        try {
            pid = std::stoll(record.project_path);
        } catch (...) {
            // ID 解析失败，pid 保持 0 或做其他处理
        }

        // [聚合] 累加项目时长 (ID -> Duration)
        temp_proj_agg[date][pid] += record.duration_seconds;

        // [转换] ID -> Path 字符串 (用于详细记录列表)
        std::vector<std::string> parts = name_cache.get_path_parts(pid);
        record.project_path = join_path_parts(parts);

        // 添加到详细记录
        day_data.detailed_records.push_back(record);
        
        // 累加单日总时长
        day_data.total_duration += record.duration_seconds;
    }
    
    // 将聚合后的项目统计填入 DailyReportData
    for(auto& [date, agg] : temp_proj_agg) {
        auto& day_data = data_map[date];
        for(auto& kv : agg) {
            day_data.project_stats.push_back(kv);
        }
    }

    // 4. 创建格式化器
    auto formatter = GenericFormatterFactory<DailyReportData>::create(format, app_config_);

    // 5. 遍历并格式化
    for (auto& [date, data] : data_map) {
        if (data.total_duration > 0) {
            // 构建树
            build_project_tree_from_ids(data.project_tree, data.project_stats, name_cache);
            
            // 格式化
            std::string formatted_report = formatter->format_report(data);
            
            auto [year, month] = parse_year_month(date);
            grouped_reports[year][month].push_back({date, formatted_report});
        }
    }

    return grouped_reports;
}