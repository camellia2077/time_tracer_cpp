// core/domain/model/range_report_data.hpp
#ifndef CORE_DOMAIN_MODEL_RANGE_REPORT_DATA_HPP_
#define CORE_DOMAIN_MODEL_RANGE_REPORT_DATA_HPP_

#include <string>
#include <vector>
#include "reports/data/model/project_tree.hpp"

struct RangeReportData {
    // 报告标题或标识，例如 "2025-01" 或 "Last 7 Days"
    std::string report_name; 

    // 时间范围 (YYYY-MM-DD)
    std::string start_date;
    std::string end_date;

    // 该范围内涵盖的自然天数 (用于计算平均值，如 7, 30, 31)
    int covered_days = 0;

    // 实际有记录的活跃天数
    int actual_active_days = 0;
    
    // 总时长 (秒)
    long long total_duration = 0;

    // 聚合统计 (Project ID -> Duration)
    std::vector<std::pair<long long, long long>> project_stats;
    
    // 构建好的项目树
    reporting::ProjectTree project_tree;
};

#endif // CORE_DOMAIN_MODEL_RANGE_REPORT_DATA_HPP_