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
    // --- 日报加载器 ---
    static DailyTexConfig loadDailyTexConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path);
    static DailyTypConfig loadDailyTypConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path);
    static DailyMdConfig  loadDailyMdConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path);

    // --- 月报加载器 ---
    static MonthlyTexConfig loadMonthlyTexConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path);
    static MonthlyTypConfig loadMonthlyTypConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path);
    static MonthlyMdConfig  loadMonthlyMdConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path);

    // --- 周期报告加载器 ---
    static PeriodTexConfig loadPeriodTexConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path);
    static PeriodTypConfig loadPeriodTypConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path);
    static PeriodMdConfig  loadPeriodMdConfig(core::interfaces::IFileSystem& fs, const std::filesystem::path& path);
};

#endif