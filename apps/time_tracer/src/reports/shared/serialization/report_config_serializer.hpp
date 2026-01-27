// reports/shared/serialization/report_config_serializer.hpp
#ifndef REPORTS_SHARED_SERIALIZATION_REPORT_CONFIG_SERIALIZER_HPP_
#define REPORTS_SHARED_SERIALIZATION_REPORT_CONFIG_SERIALIZER_HPP_

#include <string>
#include <sstream>
#include <toml++/toml.hpp>
#include "common/config/report_config_models.hpp"


/**
 * @namespace ConfigTomlSerializer
 * @brief 负责将强类型的 Config Struct 序列化为 TOML 格式字符串。
 * * @note [架构说明 / Architecture Note]
 * 为了解决 Host (EXE) 与 Plugin (DLL) 之间因编译器不同(MSVC vs MinGW)或设置不同导致的
 * C++ ABI (二进制接口) 不兼容问题，我们不能直接跨 DLL 边界传递 C++ Struct 指针。
 * * 解决方案：
 * 1. Host 端 (这里): 将 Struct 序列化为标准的 TOML 字符串。
 * 2. 传输层: 通过 C 接口 (const char*) 传递该字符串。
 * 3. DLL 端: 接收字符串，立即解析回 Struct 供业务逻辑使用。
 */
namespace ConfigTomlSerializer {
    // 递归序列化统计项 (Statistics Items)
    inline toml::table serialize_stat_item(const ReportStatisticsItem& item) {
        toml::table tbl;
        tbl.insert("label", item.label);
        tbl.insert("show", item.show);
        if (!item.db_column.empty()) {
            tbl.insert("db_column", item.db_column);
        }
        if (!item.sub_items.empty()) {
            toml::array arr;
            for (const auto& sub : item.sub_items) {
                arr.push_back(serialize_stat_item(sub));
            }
            tbl.insert("sub_items", arr);
        }
        return tbl;
    }

    // 序列化 DailyReportConfig (对应 DayMd/Tex/Typ 插件)
    inline std::string to_toml_string(const DailyReportConfig& cfg) {
        toml::table root;

        // [扁平化序列化] 直接写入 root，匹配 DLL 端构造函数 expectations
        
        // 1. Labels
        root.insert("title_prefix", cfg.labels.report_title_prefix);
        root.insert("report_title", cfg.labels.report_title_prefix); // 兼容备用
        root.insert("date_label", cfg.labels.date_label);
        root.insert("total_time_label", cfg.labels.total_time_label);
        root.insert("status_label", cfg.labels.status_label);
        root.insert("sleep_label", cfg.labels.sleep_label);
        root.insert("exercise_label", cfg.labels.exercise_label);
        root.insert("getup_time_label", cfg.labels.getup_time_label);
        root.insert("remark_label", cfg.labels.remark_label);
        root.insert("no_records_message", cfg.labels.no_records_message);
        root.insert("statistics_label", cfg.labels.statistics_label);
        root.insert("all_activities_label", cfg.labels.all_activities_label);
        root.insert("activity_remark_label", cfg.labels.activity_remark_label);
        root.insert("project_breakdown_label", cfg.labels.project_breakdown_label);
        root.insert("activity_connector", cfg.labels.activity_connector);

        // 2. Fonts
        root.insert("main_font", cfg.fonts.main_font);
        root.insert("cjk_main_font", cfg.fonts.cjk_main_font);
        root.insert("base_font", cfg.fonts.base_font);
        root.insert("base_font_size", cfg.fonts.base_font_size);
        root.insert("report_title_font_size", cfg.fonts.report_title_font_size);
        root.insert("category_title_font_size", cfg.fonts.category_title_font_size);

        // 3. Layout
        root.insert("margin_in", cfg.layout.margin_in);
        root.insert("line_spacing_em", cfg.layout.line_spacing_em);
        root.insert("list_top_sep_pt", cfg.layout.list_top_sep_pt);
        root.insert("list_item_sep_ex", cfg.layout.list_item_sep_ex);

        // 4. Keyword Colors (保持子表结构)
        if (!cfg.keyword_colors.empty()) {
            toml::table colors;
            for (const auto& [k, v] : cfg.keyword_colors) {
                colors.insert(k, v);
            }
            root.insert("keyword_colors", colors);
        }

        // 5. Statistics Items (保持数组结构)
        if (!cfg.statistics_items.empty()) {
            toml::array arr;
            for (const auto& item : cfg.statistics_items) {
                arr.push_back(serialize_stat_item(item));
            }
            root.insert("statistics_items", arr);
        }

        // [明确] 使用 TOML 格式化器生成 Key=Value 格式
        std::stringstream ss;
        ss << toml::toml_formatter{ root };
        return ss.str();
    }
    // 序列化 RangeReportConfig (对应 RangeMd/Tex/Typ 插件)
    inline std::string to_toml_string(const RangeReportConfig& cfg) {
        toml::table root;

        // 1. Labels
        root.insert("report_title_label", cfg.labels.report_title_label);
        root.insert("date_range_separator", cfg.labels.date_range_separator);
        root.insert("total_time_label", cfg.labels.total_time_label);
        root.insert("actual_days_label", cfg.labels.actual_days_label);
        root.insert("no_records_message", cfg.labels.no_records_message);
        root.insert("invalid_data_message", cfg.labels.invalid_data_message);
        root.insert("project_breakdown_label", cfg.labels.project_breakdown_label);

        // 2. Fonts
        root.insert("main_font", cfg.fonts.main_font);
        root.insert("cjk_main_font", cfg.fonts.cjk_main_font);
        root.insert("base_font", cfg.fonts.base_font); 
        root.insert("title_font", cfg.fonts.title_font); 
        root.insert("category_title_font", cfg.fonts.category_title_font);
        root.insert("base_font_size", cfg.fonts.base_font_size);
        root.insert("report_title_font_size", cfg.fonts.report_title_font_size);
        root.insert("category_title_font_size", cfg.fonts.category_title_font_size);

        // 3. Layout
        root.insert("margin_in", cfg.layout.margin_in);
        root.insert("margin_top_cm", cfg.layout.margin_top_cm);
        root.insert("margin_bottom_cm", cfg.layout.margin_bottom_cm);
        root.insert("margin_left_cm", cfg.layout.margin_left_cm);
        root.insert("margin_right_cm", cfg.layout.margin_right_cm);
        root.insert("line_spacing_em", cfg.layout.line_spacing_em);
        root.insert("list_top_sep_pt", cfg.layout.list_top_sep_pt);
        root.insert("list_item_sep_ex", cfg.layout.list_item_sep_ex);

        std::stringstream ss;
        ss << toml::toml_formatter{ root };
        return ss.str();
    }
}

#endif