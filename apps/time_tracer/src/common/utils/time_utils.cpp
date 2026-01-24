// common/utils/time_utils.cpp
#include "common/utils/time_utils.hpp"
#include <string>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <cctype>

namespace TimeUtils {

std::string formatTime(const std::string& timeStr) {
    if (timeStr.length() == 4 && timeStr.find(':') == std::string::npos) {
        return timeStr.substr(0, 2) + ":" + timeStr.substr(2, 2);
    }
    return timeStr;
}

int calculateDurationSeconds(const std::string& startTimeStr, const std::string& endTimeStr) {
    // 简单校验格式 HH:MM
    if (startTimeStr.length() != 5 || endTimeStr.length() != 5) return 0;
    try {
        int startHour = std::stoi(startTimeStr.substr(0, 2));
        int startMin = std::stoi(startTimeStr.substr(3, 2));
        int endHour = std::stoi(endTimeStr.substr(0, 2));
        int endMin = std::stoi(endTimeStr.substr(3, 2));
        
        int startSec = (startHour * 60 + startMin) * 60;
        int endSec = (endHour * 60 + endMin) * 60;
        
        // 如果结束时间小于开始时间，视为跨天（+24小时）
        if (endSec < startSec) {
            endSec += 24 * 60 * 60;
        }
        return endSec - startSec;
    } catch (...) {
        return 0;
    }
}

int calculateDurationMinutes(const std::string& startTimeStr, const std::string& endTimeStr) {
    return calculateDurationSeconds(startTimeStr, endTimeStr) / 60;
}

long long timeStringToTimestamp(const std::string& date, const std::string& time, bool is_end_time, long long start_timestamp_for_end) {
    if (date.length() < 10 || time.length() < 5) return 0;
    std::tm t = {};
    std::string datetime_str = date + " " + time;
    std::stringstream ss(datetime_str);
    ss >> std::get_time(&t, "%Y-%m-%d %H:%M");
    if (ss.fail()) return 0;
    
    long long timestamp = std::mktime(&t);
    
    // 如果是结束时间，且计算出的时间戳小于开始时间，说明跨天了，加一天
    if (is_end_time && timestamp < start_timestamp_for_end) {
        timestamp += 24 * 60 * 60;
    }
    return timestamp;
}

// --- 原有功能的简单实现或保留 ---

int time_str_to_seconds(const std::string& time_str_in) {
    // 复用 formatTime 逻辑
    std::string time_str = formatTime(time_str_in);
    if (time_str.length() != 5 || time_str[2] != ':') return 0;
    try {
        int hours = std::stoi(time_str.substr(0, 2));
        int minutes = std::stoi(time_str.substr(3, 2));
        return hours * 3600 + minutes * 60;
    } catch (...) { return 0; }
}

std::string normalize_to_date_format(const std::string& input) {
    if (input.length() == 8 && std::all_of(input.begin(), input.end(), ::isdigit)) {
        return input.substr(0, 4) + "-" + input.substr(4, 2) + "-" + input.substr(6, 2);
    }
    return input;
}

std::string normalize_to_month_format(const std::string& input) {
    if (input.length() == 6 && std::all_of(input.begin(), input.end(), ::isdigit)) {
        return input.substr(0, 4) + "-" + input.substr(4, 2);
    }
    return input;
}

}