// config/loader/toml_loader_utils.hpp
#ifndef CONFIG_LOADER_TOML_LOADER_UTILS_HPP_
#define CONFIG_LOADER_TOML_LOADER_UTILS_HPP_

#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include <toml++/toml.hpp>
#include "common/config/report_config_models.hpp"
#include "core/application/interfaces/i_file_system.hpp"

namespace fs = std::filesystem;

namespace TomlLoaderUtils {
    toml::table read_toml(core::interfaces::IFileSystem& fs, const fs::path& path);

    // Template helpers
    template <typename T>
    T get_required(const toml::table& tbl, const std::string& key) {
        if (!tbl.contains(key)) {
            throw std::runtime_error("Missing required config key: " + key);
        }
        auto val = tbl[key].value<T>();
        if (!val) {
            throw std::runtime_error("Invalid type for key: " + key);
        }
        return *val;
    }

    // Fillers
    void parse_statistics_items(const toml::array* arr, std::vector<ReportStatisticsItem>& out_items);
    void fill_tex_style(const toml::table& tbl, FontConfig& fonts, LayoutConfig& layout);
    void fill_typ_style(const toml::table& tbl, FontConfig& fonts, LayoutConfig& layout);
    void fill_daily_labels(const toml::table& tbl, DailyReportLabels& labels);
    
    // [New] Unified Range Label Filler
    void fill_range_labels(const toml::table& tbl, RangeReportLabels& labels);
    
    void fill_keyword_colors(const toml::table& tbl, std::map<std::string, std::string>& colors);
}

#endif