// validator/json/rules/date_rules.cpp
#include "validator/json/rules/date_rules.hpp"
#include <map>
#include <string>
#include <iostream>

namespace validator {
namespace json {

namespace {
    bool is_leap(int year) { return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)); }
    
    int days_in_month(int year, int month) {
        if (month < 1 || month > 12) return 0;
        if (month == 2) return is_leap(year) ? 29 : 28;
        if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
        return 31;
    }

    // [新增] 辅助函数：安全获取字符串
    std::string get_str_safe(yyjson_val* obj, const char* key, const char* def = "") {
        yyjson_val* val = yyjson_obj_get(obj, key);
        if (val && yyjson_is_str(val)) {
            return std::string(yyjson_get_str(val));
        }
        return std::string(def);
    }
}

void validateDateContinuity(yyjson_val* days_array, std::set<Error>& errors, DateCheckMode mode) {
    if (mode == DateCheckMode::None) return;
    
    // 检查是否为有效数组
    if (!days_array || !yyjson_is_arr(days_array)) return;
    if (yyjson_arr_size(days_array) == 0) return;

    // 获取第一天以确定基准月份
    yyjson_val* first_day = yyjson_arr_get_first(days_array);
    yyjson_val* first_headers = yyjson_obj_get(first_day, "headers");
    std::string first_date = get_str_safe(first_headers, "date");
    
    if (first_date.length() != 10) return;
    
    std::string yyyy_mm = first_date.substr(0, 7);
    int year = 0;
    int month = 0;
    try {
        year = std::stoi(first_date.substr(0, 4));
        month = std::stoi(first_date.substr(5, 2));
    } catch (...) {
        return; // 日期格式错误，跳过逻辑检查
    }

    std::map<std::string, std::set<int>> month_day_map;
    
    // [修改] 使用 yyjson 宏遍历数组
    size_t idx, max;
    yyjson_val* day_val;
    yyjson_arr_foreach(days_array, idx, max, day_val) {
        yyjson_val* headers = yyjson_obj_get(day_val, "headers");
        std::string date_str = get_str_safe(headers, "date");
        
        // 简单过滤：只检查属于当前月份的数据
        if (date_str.rfind(yyyy_mm, 0) == 0 && date_str.length() == 10) {
            try {
                month_day_map[yyyy_mm].insert(std::stoi(date_str.substr(8, 2)));
            } catch (...) {
                // 忽略解析错误的日期
            }
        }
    }

    // 如果该月份没有任何数据（理论上不可能，因为上面取了 first_day），直接返回
    if (month_day_map.find(yyyy_mm) == month_day_map.end()) return;

    const auto& days_found = month_day_map[yyyy_mm];
    int num_days_in_this_month = days_in_month(year, month);
    
    int check_until = num_days_in_this_month;

    // 如果是连续性检查，只检查到已存在的最后一天
    if (mode == DateCheckMode::Continuity) {
        if (days_found.empty()) return;
        check_until = *days_found.rbegin(); 
    }

    // 检查缺失的日期
    for (int d = 1; d <= check_until; ++d) {
        if (days_found.find(d) == days_found.end()) {
            std::string missing_date_str = yyyy_mm + "-" + (d < 10 ? "0" : "") + std::to_string(d);
            std::string error_msg = "Missing date detected in month " + yyyy_mm + ": " + missing_date_str;
            
            if (mode == DateCheckMode::Continuity) {
                error_msg += " (Continuity Check)";
            } else {
                error_msg += " (Completeness Check)";
            }
            
            errors.insert({0, error_msg, ErrorType::DateContinuity});
        }
    }
}

} // namespace json
} // namespace validator