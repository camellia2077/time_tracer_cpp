// validator/txt/rules/line_rules.cpp
#include "validator/txt/rules/line_rules.hpp"
#include "common/utils/string_utils.hpp"
#include <algorithm>

namespace validator {
namespace txt {

LineRules::LineRules(const ConverterConfig& config) : config_(config) {
    // 预加载所有合法的关键字，避免在验证循环中重复查找
    for(const auto& pair : config.mapper_config.text_mapping) valid_event_keywords_.insert(pair.first);
    for(const auto& pair : config.mapper_config.text_duration_mapping) valid_event_keywords_.insert(pair.first);
    
    const auto& wake_vec = config.parser_config.wake_keywords;
    wake_keywords_.insert(wake_vec.begin(), wake_vec.end());

    for(const auto& pair : config.mapper_config.top_parent_mapping) valid_event_keywords_.insert(pair.first);
    for(const auto& pair : config.mapper_config.initial_top_parents) valid_event_keywords_.insert(pair.first);
}

bool LineRules::is_year(const std::string& line) const {
    if (line.length() != 5 || line[0] != 'y') return false;
    return std::all_of(line.begin() + 1, line.end(), ::isdigit);
}

bool LineRules::is_date(const std::string& line) const {
    return line.length() == 4 && std::all_of(line.begin(), line.end(), ::isdigit);
}

bool LineRules::is_remark(const std::string& line) const {
    const std::string& prefix = config_.parser_config.remark_prefix;
    if (prefix.empty() || line.rfind(prefix, 0) != 0) return false;
    return !trim(line.substr(prefix.length())).empty();
}

bool LineRules::is_valid_event_line(const std::string& line, int line_number, std::set<Error>& errors) const {
    if (line.length() < 5 || !std::all_of(line.begin(), line.begin() + 4, ::isdigit)) {
        return false;
    }
    try {
        int hh = std::stoi(line.substr(0, 2));
        int mm = std::stoi(line.substr(2, 2));
        if (hh > 23 || mm > 59) return false;
        
        // 简单的解析逻辑，提取描述部分
        std::string remaining_line = line.substr(4);
        std::string description;
        size_t comment_pos = std::string::npos;
        const char* delimiters[] = {"//", "#", ";"};
        for (const char* delim : delimiters) {
            size_t pos = remaining_line.find(delim);
            if (pos != std::string::npos && (comment_pos == std::string::npos || pos < comment_pos)) {
                comment_pos = pos;
            }
        }
        description = trim(remaining_line.substr(0, comment_pos));
        
        if (description.empty()) return false;

        if (wake_keywords_.count(description) == 0 && valid_event_keywords_.count(description) == 0) {
             errors.insert({line_number, "Unrecognized activity '" + description + "'.", ErrorType::UnrecognizedActivity});
        }
        return true; 
    } catch (...) {
        return false;
    }
}

} 
}