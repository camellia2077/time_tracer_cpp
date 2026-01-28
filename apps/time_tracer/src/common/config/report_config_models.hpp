// common/config/report_config_models.hpp
#ifndef COMMON_CONFIG_REPORT_CONFIG_MODELS_HPP_
#define COMMON_CONFIG_REPORT_CONFIG_MODELS_HPP_

#include <map>
#include <stdexcept>
#include <string>
#include <toml++/toml.hpp>
#include <vector>

// --- Helper for inline parsing ---
namespace ConfigHelpers {
template <typename T>
T get_required(const toml::table &tbl, const std::string &key) {
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
} // namespace ConfigHelpers

// --- Shared Style Configs ---
struct FontConfig {
  std::string main_font_;
  std::string cjk_main_font_;
  std::string base_font_; // For Typst
  std::string title_font_;
  std::string category_title_font_;
  int base_font_size_ = 10;
  int report_title_font_size_ = 14;
  int category_title_font_size_ = 12;
};

struct LayoutConfig {
  double margin_in_ = 1.0;
  double margin_top_cm_ = 2.5;
  double margin_bottom_cm_ = 2.5;
  double margin_left_cm_ = 2.0;
  double margin_right_cm_ = 2.0;
  double line_spacing_em_ = 0.6;
  double list_top_sep_pt_ = 0.0;
  double list_item_sep_ex_ = 0.0;
};

// --- Labels ---

struct DailyReportLabels {
  std::string report_title_prefix_;
  std::string date_label_;
  std::string total_time_label_;
  std::string status_label_;
  std::string sleep_label_;
  std::string exercise_label_;
  std::string getup_time_label_;
  std::string remark_label_;
  std::string no_records_message_;
  std::string statistics_label_;
  std::string all_activities_label_;
  std::string activity_remark_label_;
  std::string project_breakdown_label_;
  std::string activity_connector_;
};

struct RangeReportLabels {
  std::string report_title_label_;
  std::string date_range_separator_;
  std::string total_time_label_;
  std::string actual_days_label_;
  std::string no_records_message_;
  std::string invalid_data_message_;
  std::string project_breakdown_label_;
};

struct ReportStatisticsItem {
  std::string label_;
  std::string db_column_;
  bool show_ = true;
  std::vector<ReportStatisticsItem> sub_items_;
};

// --- Config Models (Classes with Getters & Parsing) ---

struct DailyReportConfig {
  DailyReportLabels labels_;
  FontConfig fonts_;
  LayoutConfig layout_;
  std::map<std::string, std::string> keyword_colors_;
  std::vector<ReportStatisticsItem> statistics_items_;

  DailyReportConfig() = default;

  // Constructor for Formatters
  explicit DailyReportConfig(const toml::table &tbl) {
    // Simple Parsing Logic
    using namespace ConfigHelpers;
    if (auto val = tbl["title_prefix"].value<std::string>())
      labels_.report_title_prefix_ = *val;
    else
      labels_.report_title_prefix_ =
          tbl["report_title"].value_or("Daily Report");

    labels_.date_label_ = get_required<std::string>(tbl, "date_label");
    labels_.total_time_label_ =
        get_required<std::string>(tbl, "total_time_label");
    labels_.status_label_ = tbl["status_label"].value_or("Status");
    labels_.sleep_label_ = tbl["sleep_label"].value_or("Sleep");
    labels_.exercise_label_ = tbl["exercise_label"].value_or("Exercise");
    labels_.getup_time_label_ = tbl["getup_time_label"].value_or("Getup Time");
    labels_.remark_label_ = tbl["remark_label"].value_or("Remark");
    labels_.no_records_message_ =
        tbl["no_records_message"].value_or("No records.");
    labels_.statistics_label_ = tbl["statistics_label"].value_or("Statistics");
    labels_.all_activities_label_ =
        tbl["all_activities_label"].value_or("All Activities");
    labels_.activity_remark_label_ =
        tbl["activity_remark_label"].value_or("Remark");
    labels_.project_breakdown_label_ =
        tbl["project_breakdown_label"].value_or("Project Breakdown");
    labels_.activity_connector_ = tbl["activity_connector"].value_or("->");

    // Fonts & Layout (Partial mix of Tex/Typ fields, safe defaults)
    fonts_.main_font_ = tbl["main_font"].value_or("Arial");
    fonts_.cjk_main_font_ = tbl["cjk_main_font"].value_or(fonts_.main_font_);
    fonts_.base_font_ = tbl["base_font"].value_or("Arial"); // Typst
    fonts_.base_font_size_ = tbl["base_font_size"].value_or(10);
    fonts_.report_title_font_size_ = tbl["report_title_font_size"].value_or(14);
    fonts_.category_title_font_size_ =
        tbl["category_title_font_size"].value_or(12);

    layout_.margin_in_ = tbl["margin_in"].value_or(1.0);
    layout_.line_spacing_em_ = tbl["line_spacing_em"].value_or(0.65);
    layout_.list_top_sep_pt_ = tbl["list_top_sep_pt"].value_or(0.0);
    layout_.list_item_sep_ex_ = tbl["list_item_sep_ex"].value_or(0.0);

    if (const toml::table *color_tbl = tbl["keyword_colors"].as_table()) {
      for (const auto &[key, val] : *color_tbl) {
        if (auto s = val.value<std::string>())
          keyword_colors_[std::string(key.str())] = *s;
      }
    }
  }
};

struct RangeReportConfig {
  RangeReportLabels labels_;
  FontConfig fonts_;
  LayoutConfig layout_;

  RangeReportConfig() = default;

  // Constructor for Formatters (Fixes std::make_shared error)
  explicit RangeReportConfig(const toml::table &tbl) {
    using namespace ConfigHelpers;
    labels_.report_title_label_ =
        get_required<std::string>(tbl, "report_title_label");
    labels_.date_range_separator_ = tbl["date_range_separator"].value_or("to");
    labels_.total_time_label_ =
        get_required<std::string>(tbl, "total_time_label");
    labels_.actual_days_label_ =
        get_required<std::string>(tbl, "actual_days_label");
    labels_.no_records_message_ =
        get_required<std::string>(tbl, "no_records_message");
    labels_.invalid_data_message_ =
        tbl["invalid_data_message"].value_or("Invalid data range.");
    labels_.project_breakdown_label_ =
        tbl["project_breakdown_label"].value_or("Project Breakdown");

    // Fonts
    fonts_.main_font_ = tbl["main_font"].value_or("");
    fonts_.cjk_main_font_ = tbl["cjk_main_font"].value_or(fonts_.main_font_);
    fonts_.base_font_ = tbl["base_font"].value_or("");
    fonts_.title_font_ = tbl["title_font"].value_or(fonts_.base_font_);
    fonts_.category_title_font_ =
        tbl["category_title_font"].value_or(fonts_.base_font_);

    fonts_.base_font_size_ = tbl["base_font_size"].value_or(10);
    fonts_.report_title_font_size_ = tbl["report_title_font_size"].value_or(14);
    fonts_.category_title_font_size_ =
        tbl["category_title_font_size"].value_or(12);

    // Layout
    layout_.margin_in_ = tbl["margin_in"].value_or(1.0);
    layout_.margin_top_cm_ = tbl["margin_top_cm"].value_or(2.5);
    layout_.margin_bottom_cm_ = tbl["margin_bottom_cm"].value_or(2.5);
    layout_.margin_left_cm_ = tbl["margin_left_cm"].value_or(2.0);
    layout_.margin_right_cm_ = tbl["margin_right_cm"].value_or(2.0);
    layout_.line_spacing_em_ = tbl["line_spacing_em"].value_or(0.65);
    layout_.list_top_sep_pt_ = tbl["list_top_sep_pt"].value_or(0.0);
    layout_.list_item_sep_ex_ = tbl["list_item_sep_ex"].value_or(0.0);
  }

  // --- Getters for compatibility with BaseFormatters ---

  // Labels
  std::string GetReportTitleLabel() const {
    return labels_.report_title_label_;
  }
  std::string GetDateRangeSeparator() const {
    return labels_.date_range_separator_;
  }
  std::string GetTotalTimeLabel() const { return labels_.total_time_label_; }
  std::string GetActualDaysLabel() const { return labels_.actual_days_label_; }
  std::string GetNoRecordsMessage() const {
    return labels_.no_records_message_;
  }
  std::string GetInvalidDataMessage() const {
    return labels_.invalid_data_message_;
  }
  std::string GetProjectBreakdownLabel() const {
    return labels_.project_breakdown_label_;
  }

  // Fonts
  std::string GetMainFont() const { return fonts_.main_font_; }
  std::string GetCjkMainFont() const { return fonts_.cjk_main_font_; }
  std::string GetBaseFont() const { return fonts_.base_font_; }
  std::string GetTitleFont() const { return fonts_.title_font_; }
  std::string GetCategoryTitleFont() const {
    return fonts_.category_title_font_;
  }
  int GetBaseFontSize() const { return fonts_.base_font_size_; }
  int GetReportTitleFontSize() const { return fonts_.report_title_font_size_; }
  int GetCategoryTitleFontSize() const {
    return fonts_.category_title_font_size_;
  }

  // Layout
  double GetMarginIn() const { return layout_.margin_in_; }
  double GetMarginTopCm() const { return layout_.margin_top_cm_; }
  double GetMarginBottomCm() const { return layout_.margin_bottom_cm_; }
  double GetMarginLeftCm() const { return layout_.margin_left_cm_; }
  double GetMarginRightCm() const { return layout_.margin_right_cm_; }
  double GetLineSpacingEm() const { return layout_.line_spacing_em_; }
  double GetListTopSepPt() const { return layout_.list_top_sep_pt_; }
  double GetListItemSepEx() const { return layout_.list_item_sep_ex_; }
};

// --- Type Aliases ---

using DailyTexConfig = DailyReportConfig;
using DailyTypConfig = DailyReportConfig;
using DailyMdConfig = DailyReportConfig;

// Unified Range Configs
using RangeTexConfig = RangeReportConfig;
using RangeTypConfig = RangeReportConfig;
using RangeMdConfig = RangeReportConfig;

// Legacy Compatibility
using MonthlyTexConfig = RangeReportConfig;
using MonthlyTypConfig = RangeReportConfig;
using MonthlyMdConfig = RangeReportConfig;

using PeriodTexConfig = RangeReportConfig;
using PeriodTypConfig = RangeReportConfig;
using PeriodMdConfig = RangeReportConfig;

#endif // COMMON_CONFIG_REPORT_CONFIG_MODELS_HPP_
