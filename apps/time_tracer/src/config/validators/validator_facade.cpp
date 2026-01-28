// config/validators/validator_facade.cpp
// 合并自: validator/facade/config_facade.cpp +
// converter/facade/converter_facade.cpp + reports/facade/query_facade.cpp
#include "config/validators/validator_facade.hpp"
#include "config/validators/converter_validator.hpp"
#include "config/validators/plugin_validator.hpp"
#include "config/validators/report_validator.hpp"
#include "io/disk_file_system.hpp"
#include <iostream>

// ============================================================================
// ConverterFacade 实现
// ============================================================================
bool ConverterFacade::validate(const toml::table &main_tbl,
                               const toml::table &mappings_tbl,
                               const toml::table &duration_rules_tbl) const {
  std::string mappings_path_str, duration_rules_path_str;

  MainRule main_validator;
  if (!main_validator.validate(main_tbl, mappings_path_str,
                               duration_rules_path_str)) {
    return false;
  }

  MappingRule mappings_validator;
  if (!mappings_validator.validate(mappings_tbl)) {
    return false;
  }

  DurationRule duration_rules_validator;
  if (!duration_rules_validator.validate(duration_rules_tbl)) {
    return false;
  }

  std::cout
      << "[Validator] All preprocessing configuration data (TOML) is valid."
      << std::endl;
  return true;
}

// ============================================================================
// QueryFacade 实现
// ============================================================================
bool QueryFacade::validate(
    const std::vector<std::pair<std::string, toml::table>> &query_configs)
    const {
  bool all_ok = true;

  for (const auto &config_pair : query_configs) {
    const std::string &file_name = config_pair.first;
    const toml::table &config_tbl = config_pair.second;

    auto strategy =
        ConfigValidator::StrategyFactory::create_from_filename(file_name);

    if (!strategy) {
      std::cerr
          << "[Validator] Warning: No validation strategy found for file: "
          << file_name << std::endl;
      continue;
    }

    if (!strategy->validate(config_tbl, file_name)) {
      all_ok = false;
    }
  }

  if (all_ok && !query_configs.empty()) {
    std::cout << "[Validator] All query configuration files are valid."
              << std::endl;
  }

  return all_ok;
}

// ============================================================================
// ConfigFacade 实现
// ============================================================================
bool ConfigFacade::validate_converter_configs(
    const toml::table &main_tbl, const toml::table &mappings_tbl,
    const toml::table &duration_rules_tbl) const {
  ConverterFacade reprocessing_validator;
  return reprocessing_validator.validate(main_tbl, mappings_tbl,
                                         duration_rules_tbl);
}

bool ConfigFacade::validate_query_configs(
    const std::vector<std::pair<std::string, toml::table>> &query_configs)
    const {
  QueryFacade query_validator;
  return query_validator.validate(query_configs);
}

bool ConfigFacade::validate_plugins(
    const std::filesystem::path &plugins_path) const {
  const std::vector<std::string> expected_plugins = {
      "DayMdFormatter",    "DayTexFormatter",    "DayTypFormatter",
      "MonthMdFormatter",  "MonthTexFormatter",  "MonthTypFormatter",
      "PeriodMdFormatter", "PeriodTexFormatter", "PeriodTypFormatter"};

  PluginValidator validator;
  io::DiskFileSystem fs;

  bool plugins_ok = validator.validate(fs, plugins_path, expected_plugins);

  std::filesystem::path bin_dir = plugins_path.parent_path();
  bool core_ok = validator.validate(fs, bin_dir, {"reports_shared"});

  return plugins_ok && core_ok;
}
