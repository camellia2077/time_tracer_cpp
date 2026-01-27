// config/validators/report_validator.cpp
// 合并自: validator/reports/strategies/*.cpp
#include "config/validators/report_validator.hpp"
#include <iostream>
#include <set>
#include <regex>
#include <string>
#include <stdexcept>

namespace ConfigValidator {

// ============================================================================
// Helper: 递归验证 statistics_items
// ============================================================================
static bool validate_stats_recursive(const toml::array& items_array, const std::string& file_name, const std::string& context = "root") {
    for (size_t i = 0; i < items_array.size(); ++i) {
        const auto& item_node = items_array[i];
        std::string current_context = context + "[" + std::to_string(i) + "]";

        if (!item_node.is_table()) {
            std::cerr << "[Validator] Error in " << file_name << ": Item " << current_context << " must be a table." << std::endl;
            return false;
        }

        const auto& item = *item_node.as_table();

        if (!item.contains("label") || !item["label"].is_string()) {
            std::cerr << "[Validator] Error in " << file_name << ": Item " << current_context << " must contain a string 'label'." << std::endl;
            return false;
        }
        if (item.contains("show") && !item["show"].is_boolean()) {
             std::cerr << "[Validator] Error in " << file_name << ": Item " << current_context << " 'show' must be a boolean." << std::endl;
             return false;
        }
        if (item.contains("db_column") && !item["db_column"].is_string()) {
            std::cerr << "[Validator] Error in " << file_name << ": 'db_column' in " << current_context << " must be a string." << std::endl;
            return false;
        }

        if (item.contains("sub_items")) {
            if (const toml::array* sub_arr = item["sub_items"].as_array()) {
                if (!validate_stats_recursive(*sub_arr, file_name, current_context + ".sub_items")) {
                    return false;
                }
            } else {
                 std::cerr << "[Validator] Error in " << file_name << ": 'sub_items' in " << current_context << " must be an array." << std::endl;
                 return false;
            }
        }
    }
    return true;
}

// ============================================================================
// BaseStrategy 实现
// ============================================================================
bool BaseStrategy::validate(const toml::table& query_config, const std::string& file_name) const {
    if (!validate_specific_keys(query_config, file_name)) {
        return false;
    }
    if (!validate_common_rules(query_config, file_name)) {
        return false;
    }
    if (file_name.find("Day") != std::string::npos) {
        if (!validate_statistics_items(query_config, file_name)) {
            return false;
        }
    }
    return true;
}

bool BaseStrategy::validate_common_rules(const toml::table& query_config, const std::string& file_name) const {
    const std::set<std::string> numeric_keys = {
        "base_font_size", "report_title_font_size", "category_title_font_size",
        "margin_in", "margin_top_cm", "margin_bottom_cm", "margin_left_cm", "margin_right_cm",
        "list_top_sep_pt", "list_item_sep_ex", "line_spacing_em"
    };

    for (const auto& [key, node] : query_config) {
        std::string key_str(key.str());
        
        if (key_str == "statistics_items") continue;

        if (numeric_keys.count(key_str) && !node.is_number()) {
            std::cerr << "[Validator] Error in " << file_name << ": Key '" << key_str << "' must have a numeric value." << std::endl;
            return false;
        }

        if (key_str == "keyword_colors") {
            if (!node.is_table()) {
                 std::cerr << "[Validator] Error in " << file_name << ": 'keyword_colors' must be a table." << std::endl;
                 return false;
            }
            const auto& colors = *node.as_table();
            for (const auto& [color_key, color_val] : colors) {
                if (!color_val.is_string() || !is_valid_hex_color(color_val.value<std::string>().value_or(""))) {
                    std::cerr << "[Validator] Error in " << file_name << ": Color for '" << color_key.str()
                              << "' is not a valid hex string (e.g., #RRGGBB)." << std::endl;
                    return false;
                }
            }
        }
        
        if (node.is_string()) {
            std::string val_str = *node.value<std::string>();
            if (val_str.empty() && key_str != "remark" && key_str != "db_column") {
                // remark和db_column允许为空
            }
        }
    }
    return true;
}

bool BaseStrategy::validate_statistics_items(const toml::table& query_config, const std::string& file_name) const {
    if (!query_config.contains("statistics_items")) return true;

    const auto& items_node = query_config["statistics_items"];
    if (!items_node.is_array()) {
        std::cerr << "[Validator] Error in " << file_name << ": 'statistics_items' must be an array." << std::endl;
        return false;
    }

    return validate_stats_recursive(*items_node.as_array(), file_name);
}

bool BaseStrategy::is_valid_hex_color(const std::string& color_string) const {
    static const std::regex hex_color_regex(R"(^#[0-9a-fA-F]{6}$)");
    return std::regex_match(color_string, hex_color_regex);
}

// ============================================================================
// DailyTexStrategy 实现
// ============================================================================
bool DailyTexStrategy::validate_specific_keys(const toml::table& query_config, const std::string& file_name) const {
    const std::set<std::string> daily_tex_keys = {
        "report_title", "date_label", "total_time_label", "status_label", "sleep_label", "exercise_label",
        "getup_time_label", "remark_label", "no_records_message", "statistics_label",
        "all_activities_label", "activity_remark_label", "activity_connector", "keyword_colors",
        "statistics_items"
    };

    for (const auto& key : daily_tex_keys) {
        if (!query_config.contains(key)) {
            std::cerr << "[Validator] Error in " << file_name << ": Missing daily report key '" << key << "'." << std::endl;
            return false;
        }
    }
    return true;
}

// ============================================================================
// DailyTypStrategy 实现
// ============================================================================
bool DailyTypStrategy::validate_specific_keys(const toml::table& query_config, const std::string& file_name) const {
    const std::set<std::string> daily_typ_keys = {
        "title_prefix", "date_label", "total_time_label", "status_label", "sleep_label",
        "getup_time_label", "remark_label", "exercise_label", "no_records_message", "statistics_label",
        "all_activities_label", "activity_remark_label", "activity_connector", "keyword_colors",
        "statistics_items"
    };
    
    for (const auto& key : daily_typ_keys) {
        if (!query_config.contains(key)) {
            std::cerr << "[Validator] Error in " << file_name << ": Missing daily report key '" << key << "'." << std::endl;
            return false;
        }
    }
    return true;
}

// ============================================================================
// DailyMdStrategy 实现
// ============================================================================
bool DailyMdStrategy::validate_specific_keys(const toml::table& query_config, const std::string& file_name) const {
    const std::set<std::string> daily_md_keys = {
        "title_prefix", "date_label", "total_time_label", "status_label", "sleep_label", "exercise_label",
        "getup_time_label", "remark_label", "statistics_label", "all_activities_label",
        "activity_remark_label", "activity_connector", "statistics_items", "no_records_message"
    };

    for (const auto& key : daily_md_keys) {
        if (!query_config.contains(key)) {
            std::cerr << "[Validator] Error in " << file_name << ": Missing daily report key '" << key << "'." << std::endl;
            return false;
        }
    }
    return true;
}

// ============================================================================
// RangeStrategy 实现
// ============================================================================
bool RangeStrategy::validate_specific_keys(const toml::table& query_config, const std::string& file_name) const {
    bool is_valid = true;

    if (!query_config.contains("project_breakdown_label")) {
        std::cerr << "Error in " << file_name << ": Missing 'project_breakdown_label'" << std::endl;
        is_valid = false;
    }

    if (!query_config.contains("total_time_label")) {
        std::cerr << "Error in " << file_name << ": Missing 'total_time_label'" << std::endl;
        is_valid = false;
    }

    if (!query_config.contains("actual_days_label")) {
        std::cerr << "Error in " << file_name << ": Missing 'actual_days_label'" << std::endl;
        is_valid = false;
    }

    return is_valid;
}

// ============================================================================
// StrategyFactory 实现
// ============================================================================
std::unique_ptr<IQueryStrategy> StrategyFactory::create(ReportType type, ReportFormat format) {
    switch (type) {
        case ReportType::Daily:
            switch (format) {
                case ReportFormat::Markdown: return std::make_unique<DailyMdStrategy>();
                case ReportFormat::LaTeX:    return std::make_unique<DailyTexStrategy>();
                case ReportFormat::Typ:      return std::make_unique<DailyTypStrategy>();
                default: throw std::invalid_argument("Unsupported format for Daily report");
            }
        case ReportType::Monthly:
        case ReportType::Weekly:
        case ReportType::Periodic:
            return std::make_unique<RangeStrategy>();
        default:
            throw std::invalid_argument("Unsupported report type");
    }
}

std::unique_ptr<IQueryStrategy> StrategyFactory::create_from_filename(const std::string& file_name) {
    ReportType type = ReportType::Daily;
    if (file_name.find("Month") != std::string::npos) type = ReportType::Monthly;
    else if (file_name.find("Week") != std::string::npos) type = ReportType::Weekly;
    else if (file_name.find("Period") != std::string::npos) type = ReportType::Periodic;

    ReportFormat format = ReportFormat::Markdown;
    if (file_name.find("Tex") != std::string::npos) format = ReportFormat::LaTeX;
    else if (file_name.find("Typ") != std::string::npos) format = ReportFormat::Typ;

    return create(type, format);
}

} // namespace ConfigValidator
