// common/config/global_report_config.hpp
#ifndef COMMON_CONFIG_GLOBAL_REPORT_CONFIG_HPP_
#define COMMON_CONFIG_GLOBAL_REPORT_CONFIG_HPP_

#include "common/config/report_config_models.hpp" // 包含 DailyReportConfig, RangeReportConfig

// 定义每种格式 (Markdown/LaTeX/Typst) 下包含的具体报告类型配置
struct FormatReportConfig {
    DailyReportConfig day;
    RangeReportConfig month;
    RangeReportConfig week;   // [新增] Week 配置
    RangeReportConfig period; 
};

struct GlobalReportConfig {
    FormatReportConfig markdown;
    FormatReportConfig latex;
    FormatReportConfig typst;
};

#endif // COMMON_CONFIG_GLOBAL_REPORT_CONFIG_HPP_