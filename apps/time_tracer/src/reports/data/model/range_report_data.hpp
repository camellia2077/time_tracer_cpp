// reports/data/model/range_report_data.hpp
#ifndef REPORTS_DATA_MODEL_RANGE_REPORT_DATA_HPP_
#define REPORTS_DATA_MODEL_RANGE_REPORT_DATA_HPP_

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
    // 对于月报，通常是该月天数；对于周期报，是查询的天数 (days_to_query)
    int covered_days = 0;

    // 实际有记录的活跃天数
    int actual_active_days = 0;
    
    // 总时长 (秒)
    long long total_duration = 0;

    // 聚合统计 (Project ID -> Duration)
    // 用于在 Service 层快速构建树，或者供某些特定的统计分析使用
    std::vector<std::pair<long long, long long>> project_stats;
    
    // 构建好的项目树 (包含层级结构和时长)
    // 这是格式化器 (Formatter) 生成报告时的主要数据源
    reporting::ProjectTree project_tree;
};

#endif // REPORTS_DATA_MODEL_RANGE_REPORT_DATA_HPP_