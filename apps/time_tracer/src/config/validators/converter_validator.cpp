// config/validators/converter_validator.cpp
// 合并自: validator/converter/rules/*.cpp
#include "config/validators/converter_validator.hpp"
#include <iostream>
#include <set>

// ============================================================================
// MainRule 实现
// ============================================================================
bool MainRule::validate(const toml::table& main_tbl, std::string& out_mappings_path, std::string& out_duration_rules_path) const {
    const std::set<std::string> required_keys = {
        "mappings_config_path", "duration_rules_config_path", "top_parent_mapping",
        "header_order", "remark_prefix", "wake_keywords"
    };

    for (const auto& key : required_keys) {
        if (!main_tbl.contains(key)) {
            std::cerr << "[Validator] Error: Main config is missing required key: '" << key << "'" << std::endl;
            return false;
        }
    }

    if (!main_tbl["mappings_config_path"].is_string() || !main_tbl["duration_rules_config_path"].is_string()) {
        std::cerr << "[Validator] Error: 'mappings_config_path' and 'duration_rules_config_path' must be strings." << std::endl;
        return false;
    }
    if (!main_tbl["top_parent_mapping"].is_table()) {
        std::cerr << "[Validator] Error: 'top_parent_mapping' must be a Table." << std::endl;
        return false;
    }
    if (!main_tbl["header_order"].is_array() || !main_tbl["wake_keywords"].is_array()) {
        std::cerr << "[Validator] Error: 'header_order' and 'wake_keywords' must be arrays." << std::endl;
        return false;
    }
    if (!main_tbl["remark_prefix"].is_string()) {
        std::cerr << "[Validator] Error: 'remark_prefix' must be a string." << std::endl;
        return false;
    }

    out_mappings_path = *main_tbl["mappings_config_path"].value<std::string>();
    out_duration_rules_path = *main_tbl["duration_rules_config_path"].value<std::string>();
    return true;
}

// ============================================================================
// MappingRule 实现
// ============================================================================
bool MappingRule::validate(const toml::table& mappings_tbl) const {
    if (!mappings_tbl.contains("text_mappings") || !mappings_tbl["text_mappings"].is_table()) {
        std::cerr << "[Validator] Error: Mappings config must contain a 'text_mappings' table." << std::endl;
        return false;
    }
    return true;
}

// ============================================================================
// DurationRule 实现
// ============================================================================
bool DurationRule::validate(const toml::table& duration_tbl) const {
    if (!duration_tbl.contains("text_duration_mappings") || !duration_tbl["text_duration_mappings"].is_table()) {
        std::cerr << "[Validator] Error: Duration rules config must contain a 'text_duration_mappings' table." << std::endl;
        return false;
    }

    if (!duration_tbl.contains("duration_mappings") || !duration_tbl["duration_mappings"].is_table()) {
        std::cerr << "[Validator] Error: Duration rules config must contain a 'duration_mappings' table." << std::endl;
        return false;
    }

    const auto* mappings = duration_tbl["duration_mappings"].as_table();
    for (const auto& [key, node] : *mappings) {
        if (!node.is_array()) {
            std::cerr << "[Validator] Error: In duration rules, the value for key '" << key.str() << "' must be an array." << std::endl;
            return false;
        }
        
        const auto& rules_array = *node.as_array();
        for (const auto& rule_node : rules_array) {
            if (!rule_node.is_table()) {
                std::cerr << "[Validator] Error: In duration rules for '" << key.str() << "', each rule must be an inline table." << std::endl;
                return false;
            }
            const auto& rule = *rule_node.as_table();
            
            if (!rule.contains("less_than_minutes") || !rule.contains("value")) {
                std::cerr << "[Validator] Error: Rules must have 'less_than_minutes' and 'value'." << std::endl;
                return false;
            }
            if (!rule["less_than_minutes"].is_integer()) {
                std::cerr << "[Validator] Error: 'less_than_minutes' must be an integer." << std::endl;
                return false;
            }
            if (!rule["value"].is_string()) {
                std::cerr << "[Validator] Error: 'value' must be a string." << std::endl;
                return false;
            }
        }
    }
    return true;
}
