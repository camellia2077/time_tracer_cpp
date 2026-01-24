// config/loader/toml_converter_config_loader.cpp
#include "config/loader/toml_converter_config_loader.hpp"
#include "common/ansi_colors.hpp"
#include <iostream>
#include <algorithm>

TomlConverterConfigLoader::TomlConverterConfigLoader(const toml::table& config_table)
    : toml_source_(config_table) {}

bool TomlConverterConfigLoader::load(ConverterConfig& config) {
    try {
        // --- 1. Parser Config ---
        if (auto val = toml_source_["remark_prefix"].value<std::string>()) {
            config.parser_config.remark_prefix = *val;
        }
        
        // header_order 目前属于 metadata，可以放在 ParserConfig 中，或者暂时忽略（如果 TextParser 不强制依赖）
        // 在重构后的模型中，header_order 似乎暂时没有位置，或者应该加回 ParserConfig？
        // 假设 ParserConfig 不需要它（TextParser 是按行解析的），或者我们在 ParserConfig 加一个字段。
        // *检查之前的 ParserConfig 定义*：没有 header_order。
        // 如果业务逻辑不再依赖它，这里可以移除；如果还需要，请在 ParserConfig 补上。
        // 假设不需要或暂存：
        /* if (const toml::array* arr = toml_source_["header_order"].as_array()) { ... } 
        */
        
        if (const toml::array* arr = toml_source_["wake_keywords"].as_array()) {
            config.parser_config.wake_keywords.clear();
            for (const auto& elem : *arr) {
                if (auto s = elem.value<std::string>()) {
                    config.parser_config.wake_keywords.push_back(*s);
                }
            }
        }
        
        // --- 2. Mapper Config ---
        
        // 同步 wake_keywords
        config.mapper_config.wake_keywords = config.parser_config.wake_keywords;

        if (const toml::table* tbl = toml_source_["top_parent_mapping"].as_table()) {
            for (const auto& [k, v] : *tbl) {
                if (auto s = v.value<std::string>()) {
                    config.mapper_config.top_parent_mapping[std::string(k.str())] = *s;
                }
            }
        }

        if (const toml::table* tbl = toml_source_["text_mappings"].as_table()) {
            for (const auto& [k, v] : *tbl) {
                if (auto s = v.value<std::string>()) {
                    config.mapper_config.text_mapping[std::string(k.str())] = *s;
                }
            }
        }
        
        if (const toml::table* tbl = toml_source_["text_duration_mappings"].as_table()) {
            for (const auto& [k, v] : *tbl) {
                if (auto s = v.value<std::string>()) {
                    config.mapper_config.text_duration_mapping[std::string(k.str())] = *s;
                }
            }
        }

        // Duration Rules
        if (const toml::table* duration_tbl = toml_source_["duration_mappings"].as_table()) {
            for (const auto& [event_key, rules_node] : *duration_tbl) {
                if (const toml::array* rules_arr = rules_node.as_array()) {
                    std::vector<DurationMappingRule> rules;
                    for (const auto& rule_node : *rules_arr) {
                        if (const toml::table* rule_tbl = rule_node.as_table()) {
                             DurationMappingRule rule;
                             rule.less_than_minutes = rule_tbl->get("less_than_minutes")->value_or(0);
                             rule.value = rule_tbl->get("value")->value_or("");
                             rules.push_back(rule);
                        }
                    }
                    std::sort(rules.begin(), rules.end(), [](const DurationMappingRule& a, const DurationMappingRule& b) {
                        return a.less_than_minutes < b.less_than_minutes;
                    });
                    config.mapper_config.duration_mappings[std::string(event_key.str())] = rules;
                }
            }
        }

        // --- 3. Linker Config ---
        if (const toml::table* gen_tbl = toml_source_["generated_activities"].as_table()) {
            if (auto val = gen_tbl->get("sleep_project_path")->value<std::string>()) {
                config.linker_config.generated_sleep_project_path = *val;
            }
        }

    } catch (const std::exception& e) {
        std::cerr << RED_COLOR << "Error parsing configuration TOML via Loader: " << e.what() << RESET_COLOR << std::endl;
        return false;
    }
    return true;
}