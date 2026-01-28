// infrastructure/persistence/toml_config_parser.cpp
// 基础设施层：TOML 配置解析器实现
#include "infrastructure/persistence/toml_config_parser.hpp"
#include <iostream>

namespace infrastructure::persistence {

std::optional<ParsedConfigData> TomlConfigParser::Parse(
    const std::string& settings_content,
    const std::string& mapping_content) {
    
    ParsedConfigData data;
    
    // 解析顺序：映射 -> 设置
    if (!ParseMappingKeys(mapping_content, data) || 
        !ParseSettings(settings_content, data)) {
        return std::nullopt;
    }
    
    return data;
}

bool TomlConfigParser::ParseMappingKeys(const std::string& content, ParsedConfigData& data) {
    try {
        toml::table tbl = toml::parse(content);
        
        if (auto mapping_node = tbl.get_as<toml::table>("text_mappings")) {
            for (auto&& [key, value] : *mapping_node) {
                data.activities.push_back(std::string(key.str()));
            }
            return true;
        } else {
            std::cerr << "Error: [text_mappings] section not found in mapping config.\n";
            return false;
        }
    } catch (const toml::parse_error& e) {
        std::cerr << "TOML Parse Error in mapping config: " << e.description() << "\n";
        return false;
    }
}

bool TomlConfigParser::ParseSettings(const std::string& content, ParsedConfigData& data) {
    try {
        toml::table tbl = toml::parse(content);
        LoadDailyRemarks(tbl, data);
        LoadActivityRemarks(tbl, data);
        LoadWakeKeywords(tbl, data);
        return true;
    } catch (const toml::parse_error& e) {
        std::cerr << "TOML Parse Error in settings config: " << e.description() << "\n";
        return false;
    }
}

void TomlConfigParser::LoadDailyRemarks(const toml::table& tbl, ParsedConfigData& data) {
    if (auto node = tbl.get_as<toml::table>("daily_remarks")) {
        domain::model::DailyRemarkConfig remarks;
        remarks.prefix = (*node)["prefix"].value_or("");
        remarks.generation_chance = (*node)["generation_chance"].value_or(0.5);
        
        int64_t lines = (*node)["max_lines"].value_or(1);
        remarks.max_lines = static_cast<int>(lines < 1 ? 1 : lines);

        if (auto arr = (*node).get_as<toml::array>("contents")) {
            for (auto&& v : *arr) {
                remarks.contents.push_back(v.value_or(""));
            }
            data.remarks.emplace(remarks);
        }
    }
}

void TomlConfigParser::LoadActivityRemarks(const toml::table& tbl, ParsedConfigData& data) {
    if (auto node = tbl.get_as<toml::table>("activity_remarks")) {
        domain::model::ActivityRemarkConfig ar;
        ar.generation_chance = (*node)["generation_chance"].value_or(0.5);
        if (auto arr = (*node).get_as<toml::array>("contents")) {
            for (auto&& v : *arr) {
                ar.contents.push_back(v.value_or(""));
            }
            data.activity_remarks.emplace(ar);
        }
    }
}

void TomlConfigParser::LoadWakeKeywords(const toml::table& tbl, ParsedConfigData& data) {
    if (auto arr = tbl.get_as<toml::array>("wake_keywords")) {
        for (auto&& v : *arr) {
            data.wake_keywords.push_back(v.value_or(""));
        }
    } else {
        data.wake_keywords.push_back("起床");
    }
}

}  // namespace infrastructure::persistence
