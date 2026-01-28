// common/utils/time_utils.hpp
#ifndef COMMON_UTILS_TIME_UTILS_HPP_
#define COMMON_UTILS_TIME_UTILS_HPP_

#include <string>

namespace TimeUtils {

/**
 * @brief 格式化时间字符串 HHMM -> HH:MM
 * 如果已经是 HH:MM 格式则原样返回
 */
std::string FormatTime(const std::string &timeStr);

/**
 * @brief 计算两个时间点之间的分钟差 (自动处理跨天)
 * @param startTimeStr HH:MM
 * @param endTimeStr HH:MM
 */
int CalculateDurationMinutes(const std::string &startTimeStr,
                             const std::string &endTimeStr);

/**
 * @brief 计算两个时间点之间的秒数差 (自动处理跨天)
 * @param startTimeStr HH:MM
 * @param endTimeStr HH:MM
 */
int CalculateDurationSeconds(const std::string &startTimeStr,
                             const std::string &endTimeStr);

/**
 * @brief 将日期和时间转换为时间戳
 * @param date YYYY-MM-DD
 * @param time HH:MM
 * @param is_end_time 是否是结束时间（用于辅助判断跨天）
 * @param start_timestamp_for_end
 * 如果是结束时间，提供开始时间戳以确保结束时间不早于开始时间
 */
long long TimeStringToTimestamp(const std::string &date,
                                const std::string &time, bool is_end_time,
                                long long start_timestamp_for_end);

// 保留原有的辅助函数（如果有需要可以整合，这里为了兼容性保留定义）
int TimeStrToSeconds(const std::string &time_str_in);
std::string NormalizeToDateFormat(const std::string &input);
std::string NormalizeToMonthFormat(const std::string &input);
} // namespace TimeUtils

#endif // COMMON_UTILS_TIME_UTILS_HPP_
