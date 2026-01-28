// common/config/app_config.hpp
#ifndef COMMON_CONFIG_APP_CONFIG_HPP_
#define COMMON_CONFIG_APP_CONFIG_HPP_

#include "common/config/global_report_config.hpp" // [新增] 引入上面的文件
#include "common/config/models/converter_config_models.hpp"
#include "common/types/date_check_mode.hpp"
#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

struct PipelineConfig {
  fs::path interval_processor_config_path_;
  std::unordered_map<fs::path, fs::path> initial_top_parents_;
  ConverterConfig loaded_converter_config_;
};

struct ReportConfigPaths {
  // Typst
  fs::path day_typ_config_path_;
  fs::path month_typ_config_path_;
  fs::path week_typ_config_path_; // [新增]
  fs::path year_typ_config_path_; // [新增]
  fs::path recent_typ_config_path_;
  fs::path range_typ_config_path_;

  // LaTeX
  fs::path day_tex_config_path_;
  fs::path month_tex_config_path_;
  fs::path week_tex_config_path_; // [新增]
  fs::path year_tex_config_path_; // [新增]
  fs::path recent_tex_config_path_;
  fs::path range_tex_config_path_;

  // Markdown
  fs::path day_md_config_path_;
  fs::path month_md_config_path_;
  fs::path week_md_config_path_; // [新增]
  fs::path year_md_config_path_; // [新增]
  fs::path recent_md_config_path_;
  fs::path range_md_config_path_;
};

struct AppConfig {
  fs::path exe_dir_path_;
  fs::path error_log_path_;
  std::optional<fs::path> export_path_;

  bool default_save_processed_output_ = false;
  DateCheckMode default_date_check_mode_ = DateCheckMode::None;

  PipelineConfig pipeline_;
  ReportConfigPaths reports_;

  GlobalReportConfig loaded_reports_;
};

#endif // COMMON_CONFIG_APP_CONFIG_HPP_
