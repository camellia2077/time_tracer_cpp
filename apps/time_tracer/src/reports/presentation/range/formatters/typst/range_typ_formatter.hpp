// reports/presentation/range/formatters/typst/range_typ_formatter.hpp
#ifndef REPORTS_PRESENTATION_RANGE_FORMATTERS_TYPST_RANGE_TYP_FORMATTER_HPP_
#define REPORTS_PRESENTATION_RANGE_FORMATTERS_TYPST_RANGE_TYP_FORMATTER_HPP_

#include "reports/domain/model/range_report_data.hpp"
#include "reports/presentation/range/common/range_base_config.hpp"
#include "reports/shared/formatters/templates/base_typ_formatter.hpp"
#include <memory>
#include <string>
#include <toml++/toml.hpp>

/**
 * @class RangeTypFormatterConfig
 * @brief 范围报表 Typst 专用的本地配置类。
 * 补齐了 BaseTypFormatter 模板所需的全部接口。
 */
class RangeTypFormatterConfig : public RangeBaseConfig {
public:
  explicit RangeTypFormatterConfig(const toml::table &config)
      : RangeBaseConfig(config) {
    // [修复] 直接从根节点读取，并不再期待 [typst] 子表
    // 键名必须与 ConfigTomlSerializer::to_toml_string(RangeReportConfig)
    // 中保持一致

    // 1. 标题样式
    title_font_ = config["title_font"].value_or<std::string>("Noto Serif SC");
    title_font_size_ = config["report_title_font_size"].value_or<int>(18);

    // 2. 基础正文样式 (BaseTypFormatter 依赖)
    base_font_ = config["base_font"].value_or<std::string>("Noto Serif SC");
    base_font_size_ = config["base_font_size"].value_or<int>(11);
    line_spacing_em_ = config["line_spacing_em"].value_or<double>(0.65);

    // 3. 章节/分类标题样式 (BaseTypFormatter 依赖)
    category_title_font_ =
        config["category_title_font"].value_or<std::string>("Noto Serif SC");
    category_title_font_size_ =
        config["category_title_font_size"].value_or<int>(14);

    // 4. 页面边距
    margin_top_ = config["margin_top_cm"].value_or<double>(2.5);
    margin_bottom_ = config["margin_bottom_cm"].value_or<double>(2.5);
    margin_left_ = config["margin_left_cm"].value_or<double>(2.5);
    margin_right_ = config["margin_right_cm"].value_or<double>(2.5);
  }

  // --- 供本地 Formatter 使用的接口 ---
  // --- 供本地 Formatter 使用的接口 ---
  const std::string &GetTitleFont() const { return title_font_; }
  int GetReportTitleFontSize() const { return title_font_size_; }

  // --- 供 BaseTypFormatter 模板使用的接口 ---
  const std::string &GetBaseFont() const { return base_font_; }
  int GetBaseFontSize() const { return base_font_size_; }
  double GetLineSpacingEm() const { return line_spacing_em_; }

  const std::string &GetCategoryTitleFont() const {
    return category_title_font_;
  }
  int GetCategoryTitleFontSize() const { return category_title_font_size_; }

  // --- 页面布局接口 ---
  double GetMarginTopCm() const { return margin_top_; }
  double GetMarginBottomCm() const { return margin_bottom_; }
  double GetMarginLeftCm() const { return margin_left_; }
  double GetMarginRightCm() const { return margin_right_; }

private:
  std::string title_font_;
  int title_font_size_;

  std::string base_font_;
  int base_font_size_;
  double line_spacing_em_;

  std::string category_title_font_;
  int category_title_font_size_;

  double margin_top_;
  double margin_bottom_;
  double margin_left_;
  double margin_right_;
};

/**
 * @class RangeTypFormatter
 * @brief 负责范围报表的 Typst 格式化实现。
 */
class RangeTypFormatter
    : public BaseTypFormatter<RangeReportData, RangeTypFormatterConfig> {
public:
  explicit RangeTypFormatter(std::shared_ptr<RangeTypFormatterConfig> config);

protected:
  std::string validate_data(const RangeReportData &data) const override;
  bool is_empty_data(const RangeReportData &data) const override;
  int get_avg_days(const RangeReportData &data) const override;
  std::string get_no_records_msg() const override;

  void format_header_content(std::stringstream &ss,
                             const RangeReportData &data) const override;
  void format_page_setup(std::stringstream &ss) const override;
};

#endif // REPORTS_PRESENTATION_RANGE_FORMATTERS_TYPST_RANGE_TYP_FORMATTER_HPP_
