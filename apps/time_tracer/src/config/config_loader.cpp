// config/config_loader.cpp
#include "config/config_loader.hpp"
#include <iostream>
#include <stdexcept>
#include <toml++/toml.hpp>

#include "config/loaders/converter_loader.hpp"
#include "config/loaders/report_loader.hpp"
#include "config/parser_utils.hpp"

namespace fs = std::filesystem;

namespace {
void load_detailed_reports(core::interfaces::IFileSystem &fs,
                           AppConfig &config) {
  // --- Typst ---
  if (!config.reports_.day_typ_config_path_.empty())
    config.loaded_reports_.typst_.day_ = ReportConfigLoader::loadDailyTypConfig(
        fs, config.reports_.day_typ_config_path_);

  if (!config.reports_.month_typ_config_path_.empty())
    config.loaded_reports_.typst_.month_ =
        ReportConfigLoader::loadRangeTypConfig(
            fs, config.reports_.month_typ_config_path_);

  if (!config.reports_.week_typ_config_path_.empty()) // [新增]
    config.loaded_reports_.typst_.week_ =
        ReportConfigLoader::loadRangeTypConfig(
            fs, config.reports_.week_typ_config_path_);

  if (!config.reports_.period_typ_config_path_.empty())
    config.loaded_reports_.typst_.period_ =
        ReportConfigLoader::loadRangeTypConfig(
            fs, config.reports_.period_typ_config_path_);

  // --- LaTeX ---
  if (!config.reports_.day_tex_config_path_.empty())
    config.loaded_reports_.latex_.day_ = ReportConfigLoader::loadDailyTexConfig(
        fs, config.reports_.day_tex_config_path_);

  if (!config.reports_.month_tex_config_path_.empty())
    config.loaded_reports_.latex_.month_ =
        ReportConfigLoader::loadRangeTexConfig(
            fs, config.reports_.month_tex_config_path_);

  if (!config.reports_.week_tex_config_path_.empty()) // [新增]
    config.loaded_reports_.latex_.week_ =
        ReportConfigLoader::loadRangeTexConfig(
            fs, config.reports_.week_tex_config_path_);

  if (!config.reports_.period_tex_config_path_.empty())
    config.loaded_reports_.latex_.period_ =
        ReportConfigLoader::loadRangeTexConfig(
            fs, config.reports_.period_tex_config_path_);

  // --- Markdown ---
  if (!config.reports_.day_md_config_path_.empty())
    config.loaded_reports_.markdown_.day_ =
        ReportConfigLoader::loadDailyMdConfig(
            fs, config.reports_.day_md_config_path_);

  if (!config.reports_.month_md_config_path_.empty())
    config.loaded_reports_.markdown_.month_ =
        ReportConfigLoader::loadRangeMdConfig(
            fs, config.reports_.month_md_config_path_);

  if (!config.reports_.week_md_config_path_.empty()) // [新增]
    config.loaded_reports_.markdown_.week_ =
        ReportConfigLoader::loadRangeMdConfig(
            fs, config.reports_.week_md_config_path_);

  if (!config.reports_.period_md_config_path_.empty())
    config.loaded_reports_.markdown_.period_ =
        ReportConfigLoader::loadRangeMdConfig(
            fs, config.reports_.period_md_config_path_);
}
} // namespace

ConfigLoader::ConfigLoader(const std::string &exe_path_str,
                           std::shared_ptr<core::interfaces::IFileSystem> fs)
    : fs_(std::move(fs)) {
  try {
    exe_path = fs::canonical(fs::path(exe_path_str)).parent_path();
  } catch (const fs::filesystem_error &e) {
    throw std::runtime_error("Failed to determine executable directory: " +
                             std::string(e.what()));
  }

  config_dir_path = exe_path / CONFIG_DIR_NAME;
  main_config_path = config_dir_path / CONFIG_FILE_NAME;
}

std::string ConfigLoader::get_main_config_path() const {
  return main_config_path.string();
}

AppConfig ConfigLoader::load_configuration() {
  if (!fs_->Exists(main_config_path)) {
    throw std::runtime_error("Configuration file not found: " +
                             main_config_path.string());
  }

  toml::table tbl;
  try {
    tbl = TomlLoaderUtils::read_toml(*fs_, main_config_path);
  } catch (const std::exception &err) {
    throw std::runtime_error("Failed to parse config.toml: " +
                             std::string(err.what()));
  }

  AppConfig app_config;
  app_config.exe_dir_path_ = exe_path;

  // 1. 解析基础路径和设置
  ConfigParserUtils::parse_system_settings(tbl, exe_path, app_config);
  ConfigParserUtils::parse_pipeline_settings(tbl, config_dir_path, app_config);
  ConfigParserUtils::parse_report_paths(tbl, config_dir_path, app_config);

  // 2. 加载 Converter 配置
  try {
    if (!app_config.pipeline_.interval_processor_config_path_.empty()) {
      app_config.pipeline_.loaded_converter_config_ =
          ConverterConfigLoader::LoadFromFile(
              *fs_, app_config.pipeline_.interval_processor_config_path_);

      // [修复] initial_top_parents 现在位于 mapper_config 中
      for (const auto &[key, val] : app_config.pipeline_.initial_top_parents_) {
        app_config.pipeline_.loaded_converter_config_.mapper_config_
            .initial_top_parents_[key.string()] = val.string();
      }
    }
  } catch (const std::exception &e) {
    throw std::runtime_error("Failed to load converter configuration: " +
                             std::string(e.what()));
  }

  // 3. 加载报表配置
  try {
    load_detailed_reports(*fs_, app_config);
  } catch (const std::exception &e) {
    throw std::runtime_error("Failed to load report configuration details: " +
                             std::string(e.what()));
  }

  return app_config;
}
