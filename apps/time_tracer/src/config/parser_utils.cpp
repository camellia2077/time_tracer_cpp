// config/parser_utils.cpp
// 合并自: internal/config_parser_utils.cpp + loader/toml_loader_utils.cpp
#include "config/parser_utils.hpp"
#include <iostream>
#include <stdexcept>

// ============================================================================
// ConfigParserUtils 实现
// ============================================================================
namespace ConfigParserUtils {

void parse_system_settings(const toml::table& tbl, const fs::path& exe_path, AppConfig& config) {
    const toml::node_view<const toml::node> sys_node = tbl["system"];
    
    // 如果 [system] 不存在，尝试 [general]
    if (!sys_node) {
        if (tbl.contains("general")) {
            const toml::node_view<const toml::node> gen_node = tbl["general"];
             if (auto val = gen_node["error_log"].value<std::string>()) {
                config.error_log_path = exe_path / *val;
            } else {
                config.error_log_path = exe_path / "error.log";
            }
             if (auto val = gen_node["export_root"].value<std::string>()) {
                config.export_path = *val;
            }
            config.default_save_processed_output = gen_node["save_processed_output"].value_or(false);
            bool check = gen_node["date_check_continuity"].value_or(false);
            config.default_date_check_mode = check ? DateCheckMode::Continuity : DateCheckMode::None;
            return;
        }
    }

    if (sys_node) {
        if (auto val = sys_node["error_log"].value<std::string>()) {
            config.error_log_path = exe_path / *val;
        } else {
            config.error_log_path = exe_path / "error.log";
        }
        
        if (auto val = sys_node["export_root"].value<std::string>()) {
            config.export_path = *val;
        }

        config.default_save_processed_output = sys_node["save_processed_output"].value_or(false);
        
        bool check = sys_node["date_check_continuity"].value_or(false);
        config.default_date_check_mode = check ? DateCheckMode::Continuity : DateCheckMode::None;
    } else {
         config.error_log_path = exe_path / "error.log";
    }
}

void parse_pipeline_settings(const toml::table& tbl, const fs::path& config_dir, AppConfig& config) {
    if (tbl.contains("converter")) {
        const auto& proc = tbl["converter"];
        
        if (auto val = proc["interval_config"].value<std::string>()) {
            config.pipeline.interval_processor_config_path = config_dir / *val;
        } else {
             throw std::runtime_error("Missing 'interval_config' in [converter] section.");
        }
        
        if (const toml::table* parents = proc["initial_top_parents"].as_table()) {
            for (const auto& [key, val] : *parents) {
                if (auto path_str = val.value<std::string>()) {
                    config.pipeline.initial_top_parents[fs::path(key.str())] = fs::path(*path_str);
                }
            }
        }
    } else {
        throw std::runtime_error("Missing [converter] configuration block."); 
    }
}

void parse_report_paths(const toml::table& tbl, const fs::path& config_dir, AppConfig& config) {
    if (tbl.contains("reports")) {
        const auto& reports = tbl["reports"];
        
        auto load_paths = [&](const std::string& key, fs::path& day, fs::path& month, fs::path& week, fs::path& period) {
            if (reports[key].is_table()) {
                const auto& section = reports[key];
                if (auto v = section["day"].value<std::string>()) day = config_dir / *v;
                if (auto v = section["month"].value<std::string>()) month = config_dir / *v;
                if (auto v = section["week"].value<std::string>()) week = config_dir / *v;
                if (auto v = section["period"].value<std::string>()) period = config_dir / *v;
            }
        };

        load_paths("typst", 
            config.reports.day_typ_config_path, 
            config.reports.month_typ_config_path, 
            config.reports.week_typ_config_path,
            config.reports.period_typ_config_path
        );
        
        load_paths("latex", 
            config.reports.day_tex_config_path, 
            config.reports.month_tex_config_path, 
            config.reports.week_tex_config_path,
            config.reports.period_tex_config_path
        );
        
        load_paths("markdown", 
            config.reports.day_md_config_path, 
            config.reports.month_md_config_path, 
            config.reports.week_md_config_path,
            config.reports.period_md_config_path
        );

    } else {
        throw std::runtime_error("Missing [reports] configuration block.");
    }
}

} // namespace ConfigParserUtils

// ============================================================================
// TomlLoaderUtils 实现
// ============================================================================
namespace TomlLoaderUtils {

toml::table read_toml(core::interfaces::IFileSystem& fs, const fs::path& path) {
    try {
        std::string content = fs.read_content(path);
        return toml::parse(content);
    } catch (const toml::parse_error& e) {
        throw std::runtime_error("Config TOML Parse Error [" + path.string() + "]: " + std::string(e.description()));
    } catch (const std::exception& e) {
        throw std::runtime_error("Config Load Error [" + path.string() + "]: " + e.what());
    }
}

void parse_statistics_items(const toml::array* arr, std::vector<ReportStatisticsItem>& out_items) {
    if (!arr) return;
    for (const auto& node : *arr) {
        if (!node.is_table()) continue;
        const auto& tbl = *node.as_table();
        ReportStatisticsItem item;
        item.label = get_required<std::string>(tbl, "label");
        item.show = tbl["show"].value_or(true);
        item.db_column = tbl["db_column"].value_or<std::string>("");
        if (const toml::array* sub_arr = tbl["sub_items"].as_array()) {
            parse_statistics_items(sub_arr, item.sub_items);
        }
        out_items.push_back(item);
    }
}

void fill_tex_style(const toml::table& tbl, FontConfig& fonts, LayoutConfig& layout) {
    fonts.main_font = get_required<std::string>(tbl, "main_font");
    fonts.cjk_main_font = tbl["cjk_main_font"].value_or(fonts.main_font);
    fonts.base_font_size = get_required<int>(tbl, "base_font_size");
    fonts.report_title_font_size = get_required<int>(tbl, "report_title_font_size");
    fonts.category_title_font_size = get_required<int>(tbl, "category_title_font_size");
    layout.margin_in = tbl["margin_in"].value_or(1.0);
    layout.list_top_sep_pt = tbl["list_top_sep_pt"].value_or(0.0);
    layout.list_item_sep_ex = tbl["list_item_sep_ex"].value_or(0.0);
}

void fill_typ_style(const toml::table& tbl, FontConfig& fonts, LayoutConfig& layout) {
    fonts.base_font = get_required<std::string>(tbl, "base_font");
    fonts.title_font = tbl["title_font"].value_or(fonts.base_font);
    fonts.category_title_font = tbl["category_title_font"].value_or(fonts.base_font);
    fonts.base_font_size = get_required<int>(tbl, "base_font_size");
    fonts.report_title_font_size = get_required<int>(tbl, "report_title_font_size");
    fonts.category_title_font_size = get_required<int>(tbl, "category_title_font_size");
    layout.line_spacing_em = tbl["line_spacing_em"].value_or(0.65);
    layout.margin_top_cm = tbl["margin_top_cm"].value_or(2.5);
    layout.margin_bottom_cm = tbl["margin_bottom_cm"].value_or(2.5);
    layout.margin_left_cm = tbl["margin_left_cm"].value_or(2.0);
    layout.margin_right_cm = tbl["margin_right_cm"].value_or(2.0);
}

void fill_daily_labels(const toml::table& tbl, DailyReportLabels& labels) {
    if (auto val = tbl["title_prefix"].value<std::string>()) {
        labels.report_title_prefix = *val;
    } else {
        labels.report_title_prefix = tbl["report_title"].value_or("Daily Report for");
    }
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
}

void fill_range_labels(const toml::table& tbl, RangeReportLabels& labels) {
    labels.report_title_label = get_required<std::string>(tbl, "report_title_label");
    labels.date_range_separator = tbl["date_range_separator"].value_or("to");
    labels.total_time_label = get_required<std::string>(tbl, "total_time_label");
    labels.actual_days_label = get_required<std::string>(tbl, "actual_days_label");
    labels.no_records_message = get_required<std::string>(tbl, "no_records_message");
    labels.invalid_data_message = tbl["invalid_data_message"].value_or("Invalid data range.");
    labels.project_breakdown_label = tbl["project_breakdown_label"].value_or("Project Breakdown");
}

void fill_keyword_colors(const toml::table& tbl, std::map<std::string, std::string>& colors) {
    if (const toml::table* color_tbl = tbl["keyword_colors"].as_table()) {
        for (const auto& [key, val] : *color_tbl) {
            if (auto s = val.value<std::string>()) {
                colors[std::string(key.str())] = *s;
            }
        }
    }
}

} // namespace TomlLoaderUtils
