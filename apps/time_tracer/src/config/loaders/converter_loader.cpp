// config/loaders/converter_loader.cpp
// 合并自: loader/converter_config_loader.cpp +
// loader/toml_converter_config_loader.cpp
#include "config/loaders/converter_loader.hpp"
#include "common/ansi_colors.hpp"
#include "config/parser_utils.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace fs = std::filesystem;
using namespace TomlLoaderUtils;

// ============================================================================
// ConverterConfigLoader 实现
// ============================================================================

void ConverterConfigLoader::MergeTomlTable(toml::table &target,
                                           const toml::table &source) {
  for (const auto &[key, val] : source) {
    if (target.contains(key)) {
      if (target[key].is_table() && val.is_table()) {
        MergeTomlTable(*target[key].as_table(), *val.as_table());
      } else {
        target.insert_or_assign(key, val);
      }
    } else {
      target.insert(key, val);
    }
  }
}

toml::table
ConverterConfigLoader::LoadMergedToml(core::interfaces::IFileSystem &fs,
                                      const fs::path &main_config_path) {
  if (!fs.Exists(main_config_path)) {
    throw std::runtime_error("Converter config file not found: " +
                             main_config_path.string());
  }

  toml::table main_tbl = read_toml(fs, main_config_path);
  fs::path config_dir = main_config_path.parent_path();

  // 合并 mappings_config_path
  if (auto path_node = main_tbl["mappings_config_path"].value<std::string>()) {
    fs::path map_path = config_dir / *path_node;
    if (fs.Exists(map_path)) {
      auto mapping_tbl = read_toml(fs, map_path);
      if (mapping_tbl.contains("text_mappings")) {
        if (!main_tbl.contains("text_mappings"))
          main_tbl.insert("text_mappings", toml::table{});
        MergeTomlTable(*main_tbl["text_mappings"].as_table(),
                       *mapping_tbl["text_mappings"].as_table());
      }
      if (mapping_tbl.contains("text_duration_mappings")) {
        if (!main_tbl.contains("text_duration_mappings"))
          main_tbl.insert("text_duration_mappings", toml::table{});
        MergeTomlTable(*main_tbl["text_duration_mappings"].as_table(),
                       *mapping_tbl["text_duration_mappings"].as_table());
      }
    }
  }

  // 合并 duration_rules_config_path
  if (auto path_node =
          main_tbl["duration_rules_config_path"].value<std::string>()) {
    fs::path rule_path = config_dir / *path_node;
    if (fs.Exists(rule_path)) {
      auto rules_tbl = read_toml(fs, rule_path);
      if (rules_tbl.contains("duration_mappings")) {
        if (!main_tbl.contains("duration_mappings"))
          main_tbl.insert("duration_mappings", toml::table{});
        MergeTomlTable(*main_tbl["duration_mappings"].as_table(),
                       *rules_tbl["duration_mappings"].as_table());
      }
      if (rules_tbl.contains("text_duration_mappings")) {
        if (!main_tbl.contains("text_duration_mappings"))
          main_tbl.insert("text_duration_mappings", toml::table{});
        MergeTomlTable(*main_tbl["text_duration_mappings"].as_table(),
                       *rules_tbl["text_duration_mappings"].as_table());
      }
    }
  }

  return main_tbl;
}

void ConverterConfigLoader::ParseTomlToStruct(const toml::table &tbl,
                                              ConverterConfig &config) {
  // 1. 填充 ParserConfig
  if (auto val = tbl["remark_prefix"].value<std::string>()) {
    config.parser_config_.remark_prefix_ = *val;
  }

  if (const toml::array *arr = tbl["wake_keywords"].as_array()) {
    for (const auto &elem : *arr) {
      if (auto s = elem.value<std::string>()) {
        config.parser_config_.wake_keywords_.push_back(*s);
      }
    }
  }

  // 2. 填充 LinkerConfig
  if (const toml::table *gen_tbl = tbl["generated_activities"].as_table()) {
    if (auto val = gen_tbl->get("sleep_project_path")->value<std::string>()) {
      config.linker_config_.generated_sleep_project_path_ = *val;
    }
  }

  // 3. 填充 MapperConfig
  config.mapper_config_.wake_keywords_ = config.parser_config_.wake_keywords_;

  auto load_map = [&](const std::string &key,
                      std::unordered_map<std::string, std::string> &target) {
    if (const toml::table *map_tbl = tbl[key].as_table()) {
      for (const auto &[k, v] : *map_tbl) {
        if (auto s = v.value<std::string>())
          target[std::string(k.str())] = *s;
      }
    }
  };

  load_map("top_parent_mapping", config.mapper_config_.top_parent_mapping_);
  load_map("text_mappings", config.mapper_config_.text_mapping_);
  load_map("text_duration_mappings",
           config.mapper_config_.text_duration_mapping_);

  // 加载时长规则
  if (const toml::table *duration_tbl = tbl["duration_mappings"].as_table()) {
    for (const auto &[event_key, rules_node] : *duration_tbl) {
      if (const toml::array *rules_arr = rules_node.as_array()) {
        std::vector<DurationMappingRule> rules;
        for (const auto &rule_node : *rules_arr) {
          if (const toml::table *rule_tbl = rule_node.as_table()) {
            DurationMappingRule rule;
            rule.less_than_minutes_ =
                (*rule_tbl)["less_than_minutes"].value_or(0);
            rule.value_ = (*rule_tbl)["value"].value_or("");
            rules.push_back(rule);
          }
        }
        std::sort(
            rules.begin(), rules.end(),
            [](const DurationMappingRule &a, const DurationMappingRule &b) {
              return a.less_than_minutes_ < b.less_than_minutes_;
            });
        config.mapper_config_.duration_mappings_[std::string(event_key.str())] =
            rules;
      }
    }
  }
}

ConverterConfig
ConverterConfigLoader::LoadFromFile(core::interfaces::IFileSystem &fs,
                                    const fs::path &main_config_path) {
  toml::table merged_toml = LoadMergedToml(fs, main_config_path);
  ConverterConfig config;
  ParseTomlToStruct(merged_toml, config);
  return config;
}

// ============================================================================
// TomlConverterConfigLoader 实现
// ============================================================================

TomlConverterConfigLoader::TomlConverterConfigLoader(
    const toml::table &config_table)
    : toml_source_(config_table) {}

bool TomlConverterConfigLoader::load(ConverterConfig &config) {
  try {
    // --- 1. Parser Config ---
    if (auto val = toml_source_["remark_prefix"].value<std::string>()) {
      config.parser_config_.remark_prefix_ = *val;
    }

    if (const toml::array *arr = toml_source_["wake_keywords"].as_array()) {
      config.parser_config_.wake_keywords_.clear();
      for (const auto &elem : *arr) {
        if (auto s = elem.value<std::string>()) {
          config.parser_config_.wake_keywords_.push_back(*s);
        }
      }
    }

    // --- 2. Mapper Config ---
    config.mapper_config_.wake_keywords_ = config.parser_config_.wake_keywords_;

    if (const toml::table *tbl =
            toml_source_["top_parent_mapping"].as_table()) {
      for (const auto &[k, v] : *tbl) {
        if (auto s = v.value<std::string>()) {
          config.mapper_config_.top_parent_mapping_[std::string(k.str())] = *s;
        }
      }
    }

    if (const toml::table *tbl = toml_source_["text_mappings"].as_table()) {
      for (const auto &[k, v] : *tbl) {
        if (auto s = v.value<std::string>()) {
          config.mapper_config_.text_mapping_[std::string(k.str())] = *s;
        }
      }
    }

    if (const toml::table *tbl =
            toml_source_["text_duration_mappings"].as_table()) {
      for (const auto &[k, v] : *tbl) {
        if (auto s = v.value<std::string>()) {
          config.mapper_config_.text_duration_mapping_[std::string(k.str())] =
              *s;
        }
      }
    }

    // Duration Rules
    if (const toml::table *duration_tbl =
            toml_source_["duration_mappings"].as_table()) {
      for (const auto &[event_key, rules_node] : *duration_tbl) {
        if (const toml::array *rules_arr = rules_node.as_array()) {
          std::vector<DurationMappingRule> rules;
          for (const auto &rule_node : *rules_arr) {
            if (const toml::table *rule_tbl = rule_node.as_table()) {
              DurationMappingRule rule;
              rule.less_than_minutes_ =
                  rule_tbl->get("less_than_minutes")->value_or(0);
              rule.value_ = rule_tbl->get("value")->value_or("");
              rules.push_back(rule);
            }
          }
          std::sort(
              rules.begin(), rules.end(),
              [](const DurationMappingRule &a, const DurationMappingRule &b) {
                return a.less_than_minutes_ < b.less_than_minutes_;
              });
          config.mapper_config_
              .duration_mappings_[std::string(event_key.str())] = rules;
        }
      }
    }

    // --- 3. Linker Config ---
    if (const toml::table *gen_tbl =
            toml_source_["generated_activities"].as_table()) {
      if (auto val = gen_tbl->get("sleep_project_path")->value<std::string>()) {
        config.linker_config_.generated_sleep_project_path_ = *val;
      }
    }

  } catch (const std::exception &e) {
    std::cerr << kRedColor
              << "Error parsing configuration TOML via Loader: " << e.what()
              << kResetColor << std::endl;
    return false;
  }
  return true;
}
