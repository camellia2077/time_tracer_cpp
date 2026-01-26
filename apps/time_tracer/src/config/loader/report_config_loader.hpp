// config/loader/report_config_loader.hpp
#ifndef CONFIG_LOADER_REPORT_CONFIG_LOADER_HPP_
#define CONFIG_LOADER_REPORT_CONFIG_LOADER_HPP_

#include <filesystem>
#include <string>
#include <vector>
#include "common/config/report_config_models.hpp"
#include "core/application/interfaces/i_file_system.hpp"

class ReportConfigLoader {
public:
    // --- Daily Loaders ---
    static DailyTexConfig loadDailyTexConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path);
    static DailyTypConfig loadDailyTypConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path);
    static DailyMdConfig  loadDailyMdConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path);

    // --- Unified Range Loaders (Month & Period) ---
    static RangeTexConfig loadRangeTexConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path);
    static RangeTypConfig loadRangeTypConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path);
    static RangeMdConfig  loadRangeMdConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path);
};

#endif