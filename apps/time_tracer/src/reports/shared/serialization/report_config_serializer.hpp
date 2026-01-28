// reports/shared/serialization/report_config_serializer.hpp
#ifndef REPORTS_SHARED_SERIALIZATION_REPORT_CONFIG_SERIALIZER_HPP_
#define REPORTS_SHARED_SERIALIZATION_REPORT_CONFIG_SERIALIZER_HPP_

#include "common/config/report_config_models.hpp"
#include <sstream>
#include <string>
#include <toml++/toml.hpp>

/**
 * @namespace ConfigTomlSerializer
 * @brief 负责将强类型的 Config Struct 序列化为 TOML 格式字符串。
 * * @note [架构说明 / Architecture Note]
 * 为了解决 Host (EXE) 与 Plugin (DLL) 之间因编译器不同(MSVC vs
 * MinGW)或设置不同导致的 C++ ABI (二进制接口) 不兼容问题，我们不能直接跨 DLL
 * 边界传递 C++ Struct 指针。
 * * 解决方案：
 * 1. Host 端 (这里): 将 Struct 序列化为标准的 TOML 字符串。
 * 2. 传输层: 通过 C 接口 (const char*) 传递该字符串。
 * 3. DLL 端: 接收字符串，立即解析回 Struct 供业务逻辑使用。
 */
namespace ConfigTomlSerializer {
// 递归序列化统计项 (Statistics Items)
inline toml::table serialize_stat_item(const ReportStatisticsItem &item) {
  toml::table tbl;
  tbl.insert("label", item.label_);
  tbl.insert("show", item.show_);
  if (!item.db_column_.empty()) {
    tbl.insert("db_column", item.db_column_);
  }
  if (!item.sub_items_.empty()) {
    toml::array arr;
    for (const auto &sub : item.sub_items_) {
      arr.push_back(serialize_stat_item(sub));
    }
    tbl.insert("sub_items", arr);
  }
  return tbl;
}

// 序列化 DailyReportConfig (对应 DayMd/Tex/Typ 插件)
inline std::string to_toml_string(const DailyReportConfig &cfg) {
  toml::table root;

  // [扁平化序列化] 直接写入 root，匹配 DLL 端构造函数 expectations

  // 1. Labels
  root.insert("title_prefix", cfg.labels_.report_title_prefix_);
  root.insert("report_title", cfg.labels_.report_title_prefix_); // 兼容备用
  root.insert("date_label", cfg.labels_.date_label_);
  root.insert("total_time_label", cfg.labels_.total_time_label_);
  root.insert("status_label", cfg.labels_.status_label_);
  root.insert("sleep_label", cfg.labels_.sleep_label_);
  root.insert("exercise_label", cfg.labels_.exercise_label_);
  root.insert("getup_time_label", cfg.labels_.getup_time_label_);
  root.insert("remark_label", cfg.labels_.remark_label_);
  root.insert("no_records_message", cfg.labels_.no_records_message_);
  root.insert("statistics_label", cfg.labels_.statistics_label_);
  root.insert("all_activities_label", cfg.labels_.all_activities_label_);
  root.insert("activity_remark_label", cfg.labels_.activity_remark_label_);
  root.insert("project_breakdown_label", cfg.labels_.project_breakdown_label_);
  root.insert("activity_connector", cfg.labels_.activity_connector_);

  // 2. Fonts
  root.insert("main_font", cfg.fonts_.main_font_);
  root.insert("cjk_main_font", cfg.fonts_.cjk_main_font_);
  root.insert("base_font", cfg.fonts_.base_font_);
  root.insert("base_font_size", cfg.fonts_.base_font_size_);
  root.insert("report_title_font_size", cfg.fonts_.report_title_font_size_);
  root.insert("category_title_font_size", cfg.fonts_.category_title_font_size_);

  // 3. Layout
  root.insert("margin_in", cfg.layout_.margin_in_);
  root.insert("line_spacing_em", cfg.layout_.line_spacing_em_);
  root.insert("list_top_sep_pt", cfg.layout_.list_top_sep_pt_);
  root.insert("list_item_sep_ex", cfg.layout_.list_item_sep_ex_);

  // 4. Keyword Colors (保持子表结构)
  if (!cfg.keyword_colors_.empty()) {
    toml::table colors;
    for (const auto &[k, v] : cfg.keyword_colors_) {
      colors.insert(k, v);
    }
    root.insert("keyword_colors", colors);
  }

  // 5. Statistics Items (保持数组结构)
  if (!cfg.statistics_items_.empty()) {
    toml::array arr;
    for (const auto &item : cfg.statistics_items_) {
      arr.push_back(serialize_stat_item(item));
    }
    root.insert("statistics_items", arr);
  }

  // [明确] 使用 TOML 格式化器生成 Key=Value 格式
  std::stringstream ss;
  ss << toml::toml_formatter{root};
  return ss.str();
}
// 序列化 RangeReportConfig (对应 RangeMd/Tex/Typ 插件)
inline std::string to_toml_string(const RangeReportConfig &cfg) {
  toml::table root;

  // 1. Labels
  root.insert("report_title_label", cfg.labels_.report_title_label_);
  root.insert("date_range_separator", cfg.labels_.date_range_separator_);
  root.insert("total_time_label", cfg.labels_.total_time_label_);
  root.insert("actual_days_label", cfg.labels_.actual_days_label_);
  root.insert("no_records_message", cfg.labels_.no_records_message_);
  root.insert("invalid_data_message", cfg.labels_.invalid_data_message_);
  root.insert("project_breakdown_label", cfg.labels_.project_breakdown_label_);

  // 2. Fonts
  root.insert("main_font", cfg.fonts_.main_font_);
  root.insert("cjk_main_font", cfg.fonts_.cjk_main_font_);
  root.insert("base_font", cfg.fonts_.base_font_);
  root.insert("title_font", cfg.fonts_.title_font_);
  root.insert("category_title_font", cfg.fonts_.category_title_font_);
  root.insert("base_font_size", cfg.fonts_.base_font_size_);
  root.insert("report_title_font_size", cfg.fonts_.report_title_font_size_);
  root.insert("category_title_font_size", cfg.fonts_.category_title_font_size_);

  // 3. Layout
  root.insert("margin_in", cfg.layout_.margin_in_);
  root.insert("margin_top_cm", cfg.layout_.margin_top_cm_);
  root.insert("margin_bottom_cm", cfg.layout_.margin_bottom_cm_);
  root.insert("margin_left_cm", cfg.layout_.margin_left_cm_);
  root.insert("margin_right_cm", cfg.layout_.margin_right_cm_);
  root.insert("line_spacing_em", cfg.layout_.line_spacing_em_);
  root.insert("list_top_sep_pt", cfg.layout_.list_top_sep_pt_);
  root.insert("list_item_sep_ex", cfg.layout_.list_item_sep_ex_);

  std::stringstream ss;
  ss << toml::toml_formatter{root};
  return ss.str();
}
} // namespace ConfigTomlSerializer

#endif
