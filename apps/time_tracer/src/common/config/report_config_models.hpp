// common/config/report_config_models.hpp
#ifndef COMMON_CONFIG_REPORT_CONFIG_MODELS_HPP_
#define COMMON_CONFIG_REPORT_CONFIG_MODELS_HPP_

#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <toml++/toml.hpp>

// --- Helper for inline parsing ---
namespace ConfigHelpers {
    template <typename T>
    T get_required(const toml::table& tbl, const std::string& key) {
        if (!tbl.contains(key)) {
            // Fallback for optional or strictly required? 
            // For now, throw to match previous loader behavior
            throw std::runtime_error("Missing required config key: " + key);
        }
        auto val = tbl[key].value<T>();
        if (!val) {
            throw std::runtime_error("Invalid type for key: " + key);
        }
        return *val;
    }
}

// --- Shared Style Configs ---
struct FontConfig {
    std::string main_font;
    std::string cjk_main_font;
    std::string base_font; // For Typst
    std::string title_font;
    std::string category_title_font;
    int base_font_size = 10;
    int report_title_font_size = 14;
    int category_title_font_size = 12;
};

struct LayoutConfig {
    double margin_in = 1.0;
    double margin_top_cm = 2.5;
    double margin_bottom_cm = 2.5;
    double margin_left_cm = 2.0;
    double margin_right_cm = 2.0;
    double line_spacing_em = 0.6;
    double list_top_sep_pt = 0.0;
    double list_item_sep_ex = 0.0;
};

// --- Labels ---

struct DailyReportLabels {
    std::string report_title_prefix;
    std::string date_label;
    std::string total_time_label;
    std::string status_label;
    std::string sleep_label;
    std::string exercise_label;
    std::string getup_time_label;
    std::string remark_label;
    std::string no_records_message;
    std::string statistics_label;
    std::string all_activities_label;
    std::string activity_remark_label;
    std::string project_breakdown_label;
    std::string activity_connector;
};

struct RangeReportLabels {
    std::string report_title_label;
    std::string date_range_separator;
    std::string total_time_label;
    std::string actual_days_label;
    std::string no_records_message;
    std::string invalid_data_message;
    std::string project_breakdown_label;
};

struct ReportStatisticsItem {
    std::string label;
    std::string db_column;
    bool show = true;
    std::vector<ReportStatisticsItem> sub_items;
};

// --- Config Models (Classes with Getters & Parsing) ---

struct DailyReportConfig {
    DailyReportLabels labels;
    FontConfig fonts;
    LayoutConfig layout;
    std::map<std::string, std::string> keyword_colors;
    std::vector<ReportStatisticsItem> statistics_items;

    DailyReportConfig() = default;
    
    // Constructor for Formatters
    explicit DailyReportConfig(const toml::table& tbl) {
        // Simple Parsing Logic
        using namespace ConfigHelpers;
        if (auto val = tbl["title_prefix"].value<std::string>()) labels.report_title_prefix = *val;
        else labels.report_title_prefix = tbl["report_title"].value_or("Daily Report");
        
        labels.date_label = get_required<std::string>(tbl, "date_label");
        labels.total_time_label = get_required<std::string>(tbl, "total_time_label");
        labels.status_label = tbl["status_label"].value_or("Status");
        labels.sleep_label = tbl["sleep_label"].value_or("Sleep");
        labels.exercise_label = tbl["exercise_label"].value_or("Exercise");
        labels.getup_time_label = tbl["getup_time_label"].value_or("Getup Time");
        labels.remark_label = tbl["remark_label"].value_or("Remark");
        labels.no_records_message = tbl["no_records_message"].value_or("No records.");
        labels.statistics_label = tbl["statistics_label"].value_or("Statistics");
        labels.all_activities_label = tbl["all_activities_label"].value_or("All Activities");
        labels.activity_remark_label = tbl["activity_remark_label"].value_or("Remark");
        labels.project_breakdown_label = tbl["project_breakdown_label"].value_or("Project Breakdown");
        labels.activity_connector = tbl["activity_connector"].value_or("->");

        // Fonts & Layout (Partial mix of Tex/Typ fields, safe defaults)
        fonts.main_font = tbl["main_font"].value_or("Arial");
        fonts.cjk_main_font = tbl["cjk_main_font"].value_or(fonts.main_font);
        fonts.base_font = tbl["base_font"].value_or("Arial"); // Typst
        fonts.base_font_size = tbl["base_font_size"].value_or(10);
        fonts.report_title_font_size = tbl["report_title_font_size"].value_or(14);
        fonts.category_title_font_size = tbl["category_title_font_size"].value_or(12);
        
        layout.margin_in = tbl["margin_in"].value_or(1.0);
        layout.line_spacing_em = tbl["line_spacing_em"].value_or(0.65);
        layout.list_top_sep_pt = tbl["list_top_sep_pt"].value_or(0.0);
        layout.list_item_sep_ex = tbl["list_item_sep_ex"].value_or(0.0);

        if (const toml::table* color_tbl = tbl["keyword_colors"].as_table()) {
            for (const auto& [key, val] : *color_tbl) {
                if (auto s = val.value<std::string>()) keyword_colors[std::string(key.str())] = *s;
            }
        }
    }
};

struct RangeReportConfig {
    RangeReportLabels labels;
    FontConfig fonts;
    LayoutConfig layout;

    RangeReportConfig() = default;

    // Constructor for Formatters (Fixes std::make_shared error)
    explicit RangeReportConfig(const toml::table& tbl) {
        using namespace ConfigHelpers;
        labels.report_title_label = get_required<std::string>(tbl, "report_title_label");
        labels.date_range_separator = tbl["date_range_separator"].value_or("to");
        labels.total_time_label = get_required<std::string>(tbl, "total_time_label");
        labels.actual_days_label = get_required<std::string>(tbl, "actual_days_label");
        labels.no_records_message = get_required<std::string>(tbl, "no_records_message");
        labels.invalid_data_message = tbl["invalid_data_message"].value_or("Invalid data range.");
        labels.project_breakdown_label = tbl["project_breakdown_label"].value_or("Project Breakdown");

        // Fonts
        fonts.main_font = tbl["main_font"].value_or("");
        fonts.cjk_main_font = tbl["cjk_main_font"].value_or(fonts.main_font);
        fonts.base_font = tbl["base_font"].value_or(""); 
        fonts.title_font = tbl["title_font"].value_or(fonts.base_font);
        fonts.category_title_font = tbl["category_title_font"].value_or(fonts.base_font);

        fonts.base_font_size = tbl["base_font_size"].value_or(10);
        fonts.report_title_font_size = tbl["report_title_font_size"].value_or(14);
        fonts.category_title_font_size = tbl["category_title_font_size"].value_or(12);

        // Layout
        layout.margin_in = tbl["margin_in"].value_or(1.0);
        layout.margin_top_cm = tbl["margin_top_cm"].value_or(2.5);
        layout.margin_bottom_cm = tbl["margin_bottom_cm"].value_or(2.5);
        layout.margin_left_cm = tbl["margin_left_cm"].value_or(2.0);
        layout.margin_right_cm = tbl["margin_right_cm"].value_or(2.0);
        layout.line_spacing_em = tbl["line_spacing_em"].value_or(0.65);
        layout.list_top_sep_pt = tbl["list_top_sep_pt"].value_or(0.0);
        layout.list_item_sep_ex = tbl["list_item_sep_ex"].value_or(0.0);
    }

    // --- Getters for compatibility with BaseFormatters ---
    
    // Labels
    std::string get_report_title_label() const { return labels.report_title_label; }
    std::string get_date_range_separator() const { return labels.date_range_separator; }
    std::string get_total_time_label() const { return labels.total_time_label; }
    std::string get_actual_days_label() const { return labels.actual_days_label; }
    std::string get_no_records_message() const { return labels.no_records_message; }
    std::string get_invalid_data_message() const { return labels.invalid_data_message; }
    std::string get_project_breakdown_label() const { return labels.project_breakdown_label; }

    // Fonts
    std::string get_main_font() const { return fonts.main_font; }
    std::string get_cjk_main_font() const { return fonts.cjk_main_font; }
    std::string get_base_font() const { return fonts.base_font; }
    std::string get_title_font() const { return fonts.title_font; }
    std::string get_category_title_font() const { return fonts.category_title_font; }
    int get_base_font_size() const { return fonts.base_font_size; }
    int get_report_title_font_size() const { return fonts.report_title_font_size; }
    int get_category_title_font_size() const { return fonts.category_title_font_size; }

    // Layout
    double get_margin_in() const { return layout.margin_in; }
    double get_margin_top_cm() const { return layout.margin_top_cm; }
    double get_margin_bottom_cm() const { return layout.margin_bottom_cm; }
    double get_margin_left_cm() const { return layout.margin_left_cm; }
    double get_margin_right_cm() const { return layout.margin_right_cm; }
    double get_line_spacing_em() const { return layout.line_spacing_em; }
    double get_list_top_sep_pt() const { return layout.list_top_sep_pt; }
    double get_list_item_sep_ex() const { return layout.list_item_sep_ex; }
};

// --- Type Aliases ---

using DailyTexConfig = DailyReportConfig;
using DailyTypConfig = DailyReportConfig;
using DailyMdConfig  = DailyReportConfig;

// Unified Range Configs
using RangeTexConfig = RangeReportConfig;
using RangeTypConfig = RangeReportConfig;
using RangeMdConfig  = RangeReportConfig;

// Legacy Compatibility
using MonthlyTexConfig = RangeReportConfig;
using MonthlyTypConfig = RangeReportConfig;
using MonthlyMdConfig  = RangeReportConfig;

using PeriodTexConfig = RangeReportConfig;
using PeriodTypConfig = RangeReportConfig;
using PeriodMdConfig  = RangeReportConfig;

#endif // COMMON_CONFIG_REPORT_CONFIG_MODELS_HPP_