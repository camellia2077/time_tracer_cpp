// config/loader/report_config_loader.cpp
#include "config/loader/report_config_loader.hpp"
#include "toml_loader_utils.hpp" 

using namespace TomlLoaderUtils; 

// ==========================================
// Daily Loaders
// ==========================================

// [修改] 增加 IFileSystem& fs 参数，并传给 read_toml

DailyTexConfig ReportConfigLoader::loadDailyTexConfig(core::interfaces::IFileSystem& fs, const fs::path& path) {
    toml::table tbl = read_toml(fs, path); 
    DailyTexConfig config;
    
    fill_tex_style(tbl, config.fonts, config.layout);
    fill_keyword_colors(tbl, config.keyword_colors);
    fill_daily_labels(tbl, config.labels);

    if (const toml::array* arr = tbl["statistics_items"].as_array()) {
        parse_statistics_items(arr, config.statistics_items);
    }
    return config;
}

DailyTypConfig ReportConfigLoader::loadDailyTypConfig(core::interfaces::IFileSystem& fs, const fs::path& path) {
    toml::table tbl = read_toml(fs, path);
    DailyTypConfig config;
    
    fill_typ_style(tbl, config.fonts, config.layout);
    fill_keyword_colors(tbl, config.keyword_colors);
    fill_daily_labels(tbl, config.labels);

    if (const toml::array* arr = tbl["statistics_items"].as_array()) {
        parse_statistics_items(arr, config.statistics_items);
    }
    return config;
}

DailyMdConfig ReportConfigLoader::loadDailyMdConfig(core::interfaces::IFileSystem& fs, const fs::path& path) {
    toml::table tbl = read_toml(fs, path);
    DailyMdConfig config;

    fill_daily_labels(tbl, config.labels);
    
    if (const toml::array* arr = tbl["statistics_items"].as_array()) {
        parse_statistics_items(arr, config.statistics_items);
    }
    return config;
}

// ==========================================
// Monthly Loaders
// ==========================================

MonthlyTexConfig ReportConfigLoader::loadMonthlyTexConfig(core::interfaces::IFileSystem& fs, const fs::path& path) {
    toml::table tbl = read_toml(fs, path);
    MonthlyTexConfig config;
    
    fill_tex_style(tbl, config.fonts, config.layout);
    fill_monthly_labels(tbl, config.labels);
    
    return config;
}

MonthlyTypConfig ReportConfigLoader::loadMonthlyTypConfig(core::interfaces::IFileSystem& fs, const fs::path& path) {
    toml::table tbl = read_toml(fs, path);
    MonthlyTypConfig config;
    
    fill_typ_style(tbl, config.fonts, config.layout);
    fill_monthly_labels(tbl, config.labels);
    
    return config;
}

MonthlyMdConfig ReportConfigLoader::loadMonthlyMdConfig(core::interfaces::IFileSystem& fs, const fs::path& path) {
    toml::table tbl = read_toml(fs, path);
    MonthlyMdConfig config;
    
    fill_monthly_labels(tbl, config.labels);
    
    return config;
}

// ==========================================
// Period Loaders
// ==========================================

PeriodTexConfig ReportConfigLoader::loadPeriodTexConfig(core::interfaces::IFileSystem& fs, const fs::path& path) {
    toml::table tbl = read_toml(fs, path);
    PeriodTexConfig config;
    
    fill_tex_style(tbl, config.fonts, config.layout);
    fill_period_labels(tbl, config.labels);
    
    return config;
}

PeriodTypConfig ReportConfigLoader::loadPeriodTypConfig(core::interfaces::IFileSystem& fs, const fs::path& path) {
    toml::table tbl = read_toml(fs, path);
    PeriodTypConfig config;
    
    fill_typ_style(tbl, config.fonts, config.layout);
    fill_period_labels(tbl, config.labels);
    
    return config;
}

PeriodMdConfig ReportConfigLoader::loadPeriodMdConfig(core::interfaces::IFileSystem& fs, const fs::path& path) {
    toml::table tbl = read_toml(fs, path);
    PeriodMdConfig config;
    
    fill_period_labels(tbl, config.labels);
    
    return config;
}