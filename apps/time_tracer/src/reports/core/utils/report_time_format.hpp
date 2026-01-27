// reports/core/utils/report_time_format.hpp
#ifndef REPORTS_CORE_UTILS_REPORT_TIME_FORMAT_HPP_
#define REPORTS_CORE_UTILS_REPORT_TIME_FORMAT_HPP_

#include <string>
#include <utility>
#include <tuple> // 必须包含，用于 std::tuple
#include "reports/core/api/shared_api.hpp"

// 获取当前日期的字符串 (YYYY-MM-DD)
REPORTS_SHARED_API std::string get_current_date();

// 获取当前时间的字符串 (HH:MM:SS)
REPORTS_SHARED_API std::string get_current_time();

// 获取 ISO 周的起始和结束日期 (周一到周日)
// 返回: {start_date, end_date}
REPORTS_SHARED_API std::pair<std::string, std::string> get_week_date_range(int year, int week);

// 获取过去 N 天的日期范围 (包含今天)
// 例如 days=7, 今天是 2026-01-26，则返回 {2026-01-20, 2026-01-26}
REPORTS_SHARED_API std::pair<std::string, std::string> get_period_dates(int days);

// [修复] 添加缺失的声明，供 Formatter 调用
// 格式化时长 (例如 "2h 30m")
REPORTS_SHARED_API std::string time_format_duration(long long total_seconds, int avg_days);

// [修复] 添加缺失的声明
// 获取月份日期范围, 返回 {start_date, end_date, days_in_month}
REPORTS_SHARED_API std::tuple<std::string, std::string, int> get_month_date_range(const std::string& ym);

#endif // REPORTS_CORE_UTILS_REPORT_TIME_FORMAT_HPP_