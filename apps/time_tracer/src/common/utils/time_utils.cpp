// common/utils/time_utils.cpp
#include "common/utils/time_utils.hpp"
#include <algorithm>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

namespace TimeUtils {

std::string FormatTime(const std::string &time_str) {
  if (time_str.length() == 4 && time_str.find(':') == std::string::npos) {
    return time_str.substr(0, 2) + ":" + time_str.substr(2, 2);
  }
  return time_str;
}

int CalculateDurationSeconds(const std::string &start_time_str,
                             const std::string &end_time_str) {
  // 简单校验格式 HH:MM
  if (start_time_str.length() != 5 || end_time_str.length() != 5)
    return 0;
  try {
    int start_hour = std::stoi(start_time_str.substr(0, 2));
    int start_min = std::stoi(start_time_str.substr(3, 2));
    int end_hour = std::stoi(end_time_str.substr(0, 2));
    int end_min = std::stoi(end_time_str.substr(3, 2));

    int start_sec = (start_hour * 60 + start_min) * 60;
    int end_sec = (end_hour * 60 + end_min) * 60;

    // 如果结束时间小于开始时间，视为跨天（+24小时）
    if (end_sec < start_sec) {
      end_sec += 24 * 60 * 60;
    }
    return end_sec - start_sec;
  } catch (...) {
    return 0;
  }
}

int CalculateDurationMinutes(const std::string &start_time_str,
                             const std::string &end_time_str) {
  return CalculateDurationSeconds(start_time_str, end_time_str) / 60;
}

long long TimeStringToTimestamp(const std::string &date,
                                const std::string &time, bool is_end_time,
                                long long start_timestamp_for_end) {
  if (date.length() < 10 || time.length() < 5)
    return 0;
  std::tm t = {};
  std::string datetime_str = date + " " + time;
  std::stringstream ss(datetime_str);
  ss >> std::get_time(&t, "%Y-%m-%d %H:%M");
  if (ss.fail())
    return 0;

  long long timestamp = std::mktime(&t);

  // 如果是结束时间，且计算出的时间戳小于开始时间，说明跨天了，加一天
  if (is_end_time && timestamp < start_timestamp_for_end) {
    timestamp += 24 * 60 * 60;
  }
  return timestamp;
}

// --- 原有功能的简单实现或保留 ---

int TimeStrToSeconds(const std::string &time_str_in) {
  // 复用 formatTime 逻辑
  std::string time_str = FormatTime(time_str_in);
  if (time_str.length() != 5 || time_str[2] != ':')
    return 0;
  try {
    int hours = std::stoi(time_str.substr(0, 2));
    int minutes = std::stoi(time_str.substr(3, 2));
    return hours * 3600 + minutes * 60;
  } catch (...) {
    return 0;
  }
}

std::string NormalizeToDateFormat(const std::string &input) {
  if (input.length() == 8 &&
      std::all_of(input.begin(), input.end(), ::isdigit)) {
    return input.substr(0, 4) + "-" + input.substr(4, 2) + "-" +
           input.substr(6, 2);
  }
  return input;
}

std::string NormalizeToMonthFormat(const std::string &input) {
  if (input.length() == 6 &&
      std::all_of(input.begin(), input.end(), ::isdigit)) {
    return input.substr(0, 4) + "-" + input.substr(4, 2);
  }
  return input;
}

std::string NormalizeRangeStart(const std::string &input) {
  if (input.length() == 4) { // YYYY
    return input + "-01-01";
  } else if (input.length() == 6) { // YYYYMM
    return input.substr(0, 4) + "-" + input.substr(4, 2) + "-01";
  }
  return NormalizeToDateFormat(input);
}

std::string NormalizeRangeEnd(const std::string &input) {
  if (input.length() == 4) { // YYYY
    return input + "-12-31";
  } else if (input.length() == 6) { // YYYYMM
    std::string year = input.substr(0, 4);
    std::string month = input.substr(4, 2);
    // Simple logic for end of month
    if (month == "02") {
      int y = std::stoi(year);
      bool is_leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
      return year + "-02-" + (is_leap ? "29" : "28");
    } else if (month == "04" || month == "06" || month == "09" ||
               month == "11") {
      return year + "-" + month + "-30";
    } else {
      return year + "-" + month + "-31";
    }
  }
  return NormalizeToDateFormat(input);
}

} // namespace TimeUtils
