// common/config/app_config.hpp
#ifndef COMMON_CONFIG_APP_CONFIG_HPP_
#define COMMON_CONFIG_APP_CONFIG_HPP_

#include <string>
#include <filesystem>
#include <vector>
#include <optional>
#include <unordered_map>
#include "common/types/date_check_mode.hpp"
#include "common/config/models/converter_config_models.hpp"
#include "common/config/global_report_config.hpp" // [新增] 引入上面的文件

namespace fs = std::filesystem;

struct PipelineConfig {
    fs::path interval_processor_config_path;
    std::unordered_map<fs::path, fs::path> initial_top_parents;
    ConverterConfig loaded_converter_config;
};

struct ReportConfigPaths {
    // Typst
    fs::path day_typ_config_path;
    fs::path month_typ_config_path;
    fs::path week_typ_config_path; // [新增]
    fs::path period_typ_config_path;

    // LaTeX
    fs::path day_tex_config_path;
    fs::path month_tex_config_path;
    fs::path week_tex_config_path; // [新增]
    fs::path period_tex_config_path;

    // Markdown
    fs::path day_md_config_path;
    fs::path month_md_config_path;
    fs::path week_md_config_path; // [新增]
    fs::path period_md_config_path;
};

struct AppConfig {
    fs::path exe_dir_path;
    fs::path error_log_path;
    std::optional<fs::path> export_path;
    
    bool default_save_processed_output = false;
    DateCheckMode default_date_check_mode = DateCheckMode::None;

    PipelineConfig pipeline;
    ReportConfigPaths reports; 
    
    GlobalReportConfig loaded_reports;
};

#endif // COMMON_CONFIG_APP_CONFIG_HPP_