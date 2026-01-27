// config/parser_utils.hpp
// 合并�? internal/config_parser_utils.hpp + loader/toml_loader_utils.hpp
#ifndef CONFIG_PARSER_UTILS_HPP_
#define CONFIG_PARSER_UTILS_HPP_

#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include <toml++/toml.hpp>
#include "common/config/app_config.hpp"
#include "common/config/report_config_models.hpp"
#include "application/interfaces/i_file_system.hpp"

namespace fs = std::filesystem;

// ============================================================================
// ConfigParserUtils - 主配置解析工�?
// ============================================================================
namespace ConfigParserUtils {
    void parse_system_settings(const toml::table& tbl, const fs::path& exe_path, AppConfig& config);
    void parse_pipeline_settings(const toml::table& tbl, const fs::path& config_dir, AppConfig& config);
    void parse_report_paths(const toml::table& tbl, const fs::path& config_dir, AppConfig& config);
}

// ============================================================================
// TomlLoaderUtils - TOML 加载和解析工�?
// ============================================================================
namespace TomlLoaderUtils {
    toml::table read_toml(core::interfaces::IFileSystem& fs, const fs::path& path);

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

    void parse_statistics_items(const toml::array* arr, std::vector<ReportStatisticsItem>& out_items);
    void fill_tex_style(const toml::table& tbl, FontConfig& fonts, LayoutConfig& layout);
    void fill_typ_style(const toml::table& tbl, FontConfig& fonts, LayoutConfig& layout);
    void fill_daily_labels(const toml::table& tbl, DailyReportLabels& labels);
    void fill_range_labels(const toml::table& tbl, RangeReportLabels& labels);
    void fill_keyword_colors(const toml::table& tbl, std::map<std::string, std::string>& colors);
}

#endif // CONFIG_PARSER_UTILS_HPP_
