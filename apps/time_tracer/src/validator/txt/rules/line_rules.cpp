#include "validator/txt/rules/line_rules.hpp"
#include "common/utils/string_utils.hpp"
#include <algorithm>

namespace validator {
namespace txt {

LineRules::LineRules(const ConverterConfig& config) : config_(config) {
    // 1. 从 MapperConfig 获取合法的映射键值 (Text Mapping)
    const auto& text_map = config.mapper_config.text_mapping;
    for(const auto& pair : text_map) {
        valid_event_keywords_.insert(pair.first);
    }

    // 2. 从 MapperConfig 获取时长映射键值 (Duration Mapping)
    const auto& dur_text_map = config.mapper_config.text_duration_mapping;
    for(const auto& pair : dur_text_map) {
        valid_event_keywords_.insert(pair.first);
    }

    // 3. 从 ParserConfig 获取唤醒词 (Wake Keywords)
    // 唤醒词也是合法的活动描述
    const auto& wake_vec = config.parser_config.wake_keywords;
    wake_keywords_.insert(wake_vec.begin(), wake_vec.end());

    // 4. 从 MapperConfig 获取顶级父类映射 (Top Parent Mapping)
    const auto& top_map = config.mapper_config.top_parent_mapping;
    for(const auto& pair : top_map) {
        valid_event_keywords_.insert(pair.first);
    }

    // 5. 从 MapperConfig 获取运行时注入的顶级父类 (Initial Top Parents)
    for(const auto& pair : config.mapper_config.initial_top_parents) {
        valid_event_keywords_.insert(pair.first);
    }
}

bool LineRules::is_year(const std::string& line) const {
    if (line.length() != 5 || line[0] != 'y') {
        return false;
    }
    return std::all_of(line.begin() + 1, line.end(), ::isdigit);
}

bool LineRules::is_date(const std::string& line) const {
    return line.length() == 4 && std::all_of(line.begin(), line.end(), ::isdigit);
}

bool LineRules::is_remark(const std::string& line) const {
    // [Fix] 访问 parser_config
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
             errors.insert({line_number, "Unrecognized activity '" + description + "'. Please check spelling or update config file.", ErrorType::UnrecognizedActivity});
        }
        return true; 
    } catch (const std::exception&) {
        return false;
    }
}

} // namespace txt
} // namespace validator