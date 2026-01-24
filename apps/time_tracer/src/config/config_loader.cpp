// config/config_loader.cpp
#include "config/config_loader.hpp"
#include <iostream>
#include <stdexcept>
#include <toml++/toml.hpp>

// [移除] #include "io/core/file_system_helper.hpp"
#include "config/loader/report_config_loader.hpp"
#include "config/internal/config_parser_utils.hpp"
#include "config/loader/converter_config_loader.hpp" 
#include "config/loader/toml_loader_utils.hpp" // 使用 read_toml

namespace fs = std::filesystem;

namespace {
    // 辅助函数：传递 fs 引用
    void load_detailed_reports(core::interfaces::IFileSystem& fs, AppConfig& config) {
        // --- Typst ---
        if (!config.reports.day_typ_config_path.empty())
            config.loaded_reports.typst.day = ReportConfigLoader::loadDailyTypConfig(fs, config.reports.day_typ_config_path);
        if (!config.reports.month_typ_config_path.empty())
            config.loaded_reports.typst.month = ReportConfigLoader::loadMonthlyTypConfig(fs, config.reports.month_typ_config_path);
        if (!config.reports.period_typ_config_path.empty())
            config.loaded_reports.typst.period = ReportConfigLoader::loadPeriodTypConfig(fs, config.reports.period_typ_config_path);

        // --- LaTeX ---
        if (!config.reports.day_tex_config_path.empty())
            config.loaded_reports.latex.day = ReportConfigLoader::loadDailyTexConfig(fs, config.reports.day_tex_config_path);
        if (!config.reports.month_tex_config_path.empty())
            config.loaded_reports.latex.month = ReportConfigLoader::loadMonthlyTexConfig(fs, config.reports.month_tex_config_path);
        if (!config.reports.period_tex_config_path.empty())
            config.loaded_reports.latex.period = ReportConfigLoader::loadPeriodTexConfig(fs, config.reports.period_tex_config_path);

        // --- Markdown ---
        if (!config.reports.day_md_config_path.empty())
            config.loaded_reports.markdown.day = ReportConfigLoader::loadDailyMdConfig(fs, config.reports.day_md_config_path);
        if (!config.reports.month_md_config_path.empty())
            config.loaded_reports.markdown.month = ReportConfigLoader::loadMonthlyMdConfig(fs, config.reports.month_md_config_path);
        if (!config.reports.period_md_config_path.empty())
            config.loaded_reports.markdown.period = ReportConfigLoader::loadPeriodMdConfig(fs, config.reports.period_md_config_path);
    }
}

ConfigLoader::ConfigLoader(const std::string& exe_path_str, std::shared_ptr<core::interfaces::IFileSystem> fs) 
    : fs_(std::move(fs)) {
    try {
        exe_path = fs::canonical(fs::path(exe_path_str)).parent_path();
    } catch (const fs::filesystem_error& e) {
        throw std::runtime_error("Failed to determine executable directory: " + std::string(e.what()));
    }

    config_dir_path = exe_path / CONFIG_DIR_NAME;
    main_config_path = config_dir_path / CONFIG_FILE_NAME;
}

std::string ConfigLoader::get_main_config_path() const {
    return main_config_path.string();
}

AppConfig ConfigLoader::load_configuration() {
    if (!fs_->exists(main_config_path)) {
        throw std::runtime_error("Configuration file not found: " + main_config_path.string());
    }

    toml::table tbl;
    try {
        // [修改] 使用 fs 读取
        tbl = TomlLoaderUtils::read_toml(*fs_, main_config_path);
    } catch (const std::exception& err) {
        throw std::runtime_error("Failed to parse config.toml: " + std::string(err.what()));
    }

    AppConfig app_config;
    app_config.exe_dir_path = exe_path;

    // 1. 解析基础路径和设置 (纯内存操作，ConfigParserUtils 无需修改)
    ConfigParserUtils::parse_system_settings(tbl, exe_path, app_config);
    ConfigParserUtils::parse_pipeline_settings(tbl, config_dir_path, app_config);
    ConfigParserUtils::parse_report_paths(tbl, config_dir_path, app_config);

    // 2. 加载 Converter 配置
    try {
        if (!app_config.pipeline.interval_processor_config_path.empty()) {
            // [修改] 传递 *fs_
            app_config.pipeline.loaded_converter_config = 
                ConverterConfigLoader::load_from_file(*fs_, app_config.pipeline.interval_processor_config_path);
            
            for (const auto& [key, val] : app_config.pipeline.initial_top_parents) {
                app_config.pipeline.loaded_converter_config.initial_top_parents[key.string()] = val.string();
            }
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load converter configuration: " + std::string(e.what()));
    }

    // 3. 加载报表配置
    try {
        // [修改] 传递 *fs_
        load_detailed_reports(*fs_, app_config);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load report configuration details: " + std::string(e.what()));
    }

    return app_config;
}