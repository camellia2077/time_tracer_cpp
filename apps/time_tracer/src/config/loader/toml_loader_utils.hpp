// config/loader/toml_loader_utils.hpp
#ifndef CONFIG_LOADER_TOML_LOADER_UTILS_HPP_
#define CONFIG_LOADER_TOML_LOADER_UTILS_HPP_

#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include <toml++/toml.hpp>
#include "common/config/report_config_models.hpp"
#include "core/application/interfaces/i_file_system.hpp" // [新增] 依赖接口

namespace fs = std::filesystem;

namespace TomlLoaderUtils {

    // --- IO ---
    /**
     * @brief 使用提供的文件系统接口读取并解析 TOML 文件
     * @param fs 文件系统接口
     * @param path 文件路径
     * @throws std::runtime_error 如果文件读取或解析失败
     */
    toml::table read_toml(core::interfaces::IFileSystem& fs, const fs::path& path);

    // --- 模板辅助函数 (保持不变) ---
    template <typename T>
    T get_optional(const toml::node_view<const toml::node>& node, const T& default_value) {
        return node.value_or(default_value);
    }

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

    // --- 逻辑填充辅助函数 (保持不变) ---
    void parse_statistics_items(const toml::array* arr, std::vector<ReportStatisticsItem>& out_items);
    void fill_tex_style(const toml::table& tbl, FontConfig& fonts, LayoutConfig& layout);
    void fill_typ_style(const toml::table& tbl, FontConfig& fonts, LayoutConfig& layout);
    void fill_daily_labels(const toml::table& tbl, DailyReportLabels& labels);
    void fill_monthly_labels(const toml::table& tbl, MonthlyReportLabels& labels);
    void fill_period_labels(const toml::table& tbl, PeriodReportLabels& labels);
    void fill_keyword_colors(const toml::table& tbl, std::map<std::string, std::string>& colors);
}

#endif // CONFIG_LOADER_TOML_LOADER_UTILS_HPP_