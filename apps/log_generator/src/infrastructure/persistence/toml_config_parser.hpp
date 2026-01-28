// infrastructure/persistence/toml_config_parser.hpp
// 基础设施层：TOML 配置解析器
#pragma once
#ifndef INFRASTRUCTURE_PERSISTENCE_TOML_CONFIG_PARSER_H_
#define INFRASTRUCTURE_PERSISTENCE_TOML_CONFIG_PARSER_H_

#include "domain/model/remark_config.hpp"
#include <string>
#include <vector>
#include <optional>
#include <toml++/toml.hpp>

namespace infrastructure::persistence {

/**
 * @struct ParsedConfigData
 * @brief TOML 文件解析后的原始数据结构
 */
struct ParsedConfigData {
    std::vector<std::string> activities;
    std::optional<domain::model::DailyRemarkConfig> remarks;
    std::optional<domain::model::ActivityRemarkConfig> activity_remarks;
    std::vector<std::string> wake_keywords;
};

/**
 * @class TomlConfigParser
 * @brief TOML 配置文件解析器
 * 
 * 职责：
 * - 解析 activities 和 mapping 配置文件
 * - 提取活动列表、备注配置等信息
 */
class TomlConfigParser {
public:
    std::optional<ParsedConfigData> Parse(
        const std::string& settings_content,
        const std::string& mapping_content
    );

private:
    bool ParseMappingKeys(const std::string& content, ParsedConfigData& data);
    bool ParseSettings(const std::string& content, ParsedConfigData& data);

    void LoadDailyRemarks(const toml::table& tbl, ParsedConfigData& data);
    void LoadActivityRemarks(const toml::table& tbl, ParsedConfigData& data);
    void LoadWakeKeywords(const toml::table& tbl, ParsedConfigData& data);
};

}  // namespace infrastructure::persistence

#endif  // INFRASTRUCTURE_PERSISTENCE_TOML_CONFIG_PARSER_H_
