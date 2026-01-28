// common/config/global_report_config.hpp
#ifndef COMMON_CONFIG_GLOBAL_REPORT_CONFIG_HPP_
#define COMMON_CONFIG_GLOBAL_REPORT_CONFIG_HPP_

#include "common/config/report_config_models.hpp" // 包含 DailyReportConfig, RangeReportConfig

// 定义每种格式 (Markdown/LaTeX/Typst) 下包含的具体报告类型配置
struct FormatReportConfig {
  DailyReportConfig day_;
  RangeReportConfig month_;
  RangeReportConfig week_; // [新增] Week 配置
  RangeReportConfig year_; // [新增] Year 配置
  RangeReportConfig recent_;
  RangeReportConfig range_;
};

struct GlobalReportConfig {
  FormatReportConfig markdown_;
  FormatReportConfig latex_;
  FormatReportConfig typst_;
};

#endif // COMMON_CONFIG_GLOBAL_REPORT_CONFIG_HPP_
