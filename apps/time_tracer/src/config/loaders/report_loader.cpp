// config/loaders/report_loader.cpp
// 移动自: loader/report_config_loader.cpp
#include "config/loaders/report_loader.hpp"
#include "config/parser_utils.hpp"

using namespace TomlLoaderUtils;

// --- Daily Loaders ---

DailyTexConfig ReportConfigLoader::loadDailyTexConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path) {
    auto tbl = read_toml(fs, path);
    DailyTexConfig config(tbl);
    return config;
}

DailyTypConfig ReportConfigLoader::loadDailyTypConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path) {
    auto tbl = read_toml(fs, path);
    DailyTypConfig config(tbl);
    return config;
}

DailyMdConfig ReportConfigLoader::loadDailyMdConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path) {
    auto tbl = read_toml(fs, path);
    DailyMdConfig config(tbl);
    return config;
}

// --- Range Loaders (Unified) ---

RangeTexConfig ReportConfigLoader::loadRangeTexConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path) {
    auto tbl = read_toml(fs, path);
    RangeTexConfig config(tbl);
    return config;
}

RangeTypConfig ReportConfigLoader::loadRangeTypConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path) {
    auto tbl = read_toml(fs, path);
    RangeTypConfig config(tbl);
    return config;
}

RangeMdConfig ReportConfigLoader::loadRangeMdConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path) {
    auto tbl = read_toml(fs, path);
    RangeMdConfig config(tbl);
    return config;
}
