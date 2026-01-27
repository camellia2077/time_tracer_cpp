// reports/core/utils/report_time_format.cpp
#include "reports/core/utils/report_time_format.hpp"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <tuple>
#include <cstdio> // for snprintf

std::pair<std::string, std::string> get_period_dates(int days) {
    auto now = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(now);
    std::tm end_tm = *std::localtime(&end_time);

    // 计算开始时间: EndDate - (days - 1)
    std::tm start_tm = end_tm;
    start_tm.tm_mday -= (days - 1);
    
    std::mktime(&start_tm);

    std::ostringstream start_ss, end_ss;
    start_ss << std::put_time(&start_tm, "%Y-%m-%d");
    end_ss << std::put_time(&end_tm, "%Y-%m-%d");

    return {start_ss.str(), end_ss.str()};
}

std::string time_format_duration(long long total_seconds, int avg_days) {
    if (total_seconds == 0) {
        if (avg_days > 1) return "0m (average: 0m/day)";
        return "0m";
    }

    long long seconds_per_day = (avg_days > 1) ? (total_seconds / avg_days) : total_seconds;

    auto format_single_duration = [](long long s) {
        if (s == 0) return std::string("0m");
        long long h = s / 3600;
        long long m = (s % 3600) / 60;
        std::stringstream ss;
        if (h > 0) {
            ss << h << "h";
        }
        if (m > 0 || h == 0) {
            if (h > 0) ss << " ";
            ss << m << "m";
        }
        return ss.str();
    };

    std::string main_duration_str = format_single_duration(total_seconds);

    if (avg_days > 1) {
        std::string avg_duration_str = format_single_duration(seconds_per_day);
        main_duration_str += " (average: " + avg_duration_str + "/day)";
    }
    return main_duration_str;
}

// 内部辅助函数，未暴露在头文件
std::string add_days_to_date_str(std::string date_str, int days) {
    if (date_str.length() != 10) return date_str;

    int year = std::stoi(date_str.substr(0, 4));
    int month = std::stoi(date_str.substr(5, 2));
    int day = std::stoi(date_str.substr(8, 2));

    std::tm t{};
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day + days;
    t.tm_isdst = -1;

    std::mktime(&t);

    std::stringstream ss;
    ss << std::put_time(&t, "%Y-%m-%d");
    return ss.str();
}

// [修复] 重命名以匹配头文件声明: get_current_date()
std::string get_current_date() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
    return ss.str();
}

// [修复] 实现 get_current_time
std::string get_current_time() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%H:%M:%S");
    return ss.str();
}

std::tuple<std::string, std::string, int> get_month_date_range(const std::string& ym) {
    if (ym.size() != 7 || ym[4] != '-') return {ym, ym, 0};
    
    int year = 0; 
    int month = 0;
    try {
        year = std::stoi(ym.substr(0, 4));
        month = std::stoi(ym.substr(5, 2));
    } catch (...) {
        return {ym, ym, 0};
    }
    
    int days_in_month = 31;
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        days_in_month = 30;
    } else if (month == 2) {
        bool is_leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        days_in_month = is_leap ? 29 : 28;
    }

    std::string start = ym + "-01";
    
    char buf[16];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d", year, month, days_in_month);
    std::string end(buf);

    return {start, end, days_in_month};
}

std::pair<std::string, std::string> get_week_date_range(int year, int week) {
    std::tm t = {};
    t.tm_year = year - 1900;
    t.tm_mon = 0; // Jan
    t.tm_mday = 4;
    t.tm_isdst = -1;
    
    std::mktime(&t);
    
    int current_wday_iso = (t.tm_wday == 0) ? 7 : t.tm_wday;
    int days_to_monday = current_wday_iso - 1;
    
    t.tm_mday -= days_to_monday;
    t.tm_mday += (week - 1) * 7;
    
    std::mktime(&t);
    
    char buf[16];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", &t);
    std::string start_date = buf;
    
    t.tm_mday += 6;
    std::mktime(&t);
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", &t);
    std::string end_date = buf;
    
    return {start_date, end_date};
}