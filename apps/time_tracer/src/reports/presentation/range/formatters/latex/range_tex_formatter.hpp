// reports/presentation/range/formatters/latex/range_tex_formatter.hpp
#ifndef REPORTS_PRESENTATION_RANGE_FORMATTERS_LATEX_RANGE_TEX_FORMATTER_HPP_
#define REPORTS_PRESENTATION_RANGE_FORMATTERS_LATEX_RANGE_TEX_FORMATTER_HPP_

#include "reports/domain/model/range_report_data.hpp"
#include "reports/presentation/range/common/range_base_config.hpp"
#include "reports/shared/config/export_style_config.hpp"
#include "reports/shared/formatters/templates/base_tex_formatter.hpp"
#include <map>
#include <memory>
#include <string>
#include <toml++/toml.hpp>

/**
 * @class RangeTexFormatterConfig
 * @brief 范围报表专用的本地配置类。
 */
class RangeTexFormatterConfig : public RangeBaseConfig {
public:
  explicit RangeTexFormatterConfig(const toml::table &config)
      : RangeBaseConfig(config), style_(config) {}

  // 字体与样式代理方法 (供 BaseTexFormatter 渲染导言区和项目树调用)
  // 字体与样式代理方法 (供 BaseTexFormatter 渲染导言区和项目树调用)
  const std::string &GetMainFont() const { return style_.get_tex_main_font(); }
  const std::string &GetCjkMainFont() const {
    return style_.get_tex_cjk_main_font();
  }
  int GetBaseFontSize() const { return style_.get_base_font_size(); }
  int GetReportTitleFontSize() const {
    return style_.get_report_title_font_size();
  }

  // [修复] 补齐 BaseTexFormatter 依赖的分类标题字号接口
  int GetCategoryTitleFontSize() const {
    return style_.get_category_title_font_size();
  }

  double GetMarginIn() const { return style_.get_tex_margin_in(); }

  // 列表间距代理
  double GetListTopSepPt() const { return style_.get_tex_list_top_sep_pt(); }
  double GetListItemSepEx() const { return style_.get_tex_list_item_sep_ex(); }

  // 满足基类模板接口要求
  std::map<std::string, std::string> GetKeywordColors() const { return {}; }

private:
  ExportStyleConfig style_;
};

/**
 * @class RangeTexFormatter
 * @brief 负责范围报表（周报/月报等）的 LaTeX 格式化。
 */
class RangeTexFormatter
    : public BaseTexFormatter<RangeReportData, RangeTexFormatterConfig> {
public:
  explicit RangeTexFormatter(std::shared_ptr<RangeTexFormatterConfig> config);

protected:
  // --- 实现基类钩子 ---
  std::string validate_data(const RangeReportData &data) const override;
  bool is_empty_data(const RangeReportData &data) const override;
  int get_avg_days(const RangeReportData &data) const override;
  std::string get_no_records_msg() const override;
  void format_header_content(std::stringstream &ss,
                             const RangeReportData &data) const override;

private:
  // 内部渲染逻辑 (原 RangeTexUtils)
  void display_summary(std::stringstream &ss,
                       const RangeReportData &data) const;
};

#endif // REPORTS_PRESENTATION_RANGE_FORMATTERS_LATEX_RANGE_TEX_FORMATTER_HPP_
