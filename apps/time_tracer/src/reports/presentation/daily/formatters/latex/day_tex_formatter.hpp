// reports/presentation/daily/formatters/latex/day_tex_formatter.hpp
#ifndef REPORTS_PRESENTATION_DAILY_FORMATTERS_LATEX_DAY_TEX_FORMATTER_HPP_
#define REPORTS_PRESENTATION_DAILY_FORMATTERS_LATEX_DAY_TEX_FORMATTER_HPP_

#include "reports/domain/model/daily_report_data.hpp"
#include "reports/presentation/daily/common/day_base_config.hpp"
#include "reports/shared/config/export_style_config.hpp"
#include "reports/shared/formatters/templates/base_tex_formatter.hpp"
#include <map>
#include <string>
#include <toml++/toml.hpp>

// [合并] 将配置类定义移入此处
class DayTexConfig : public DayBaseConfig {
public:
  explicit DayTexConfig(const toml::table &config);

  const std::string &GetReportTitle() const { return report_title_; }
  const std::map<std::string, std::string> &GetKeywordColors() const {
    return keyword_colors_;
  }

  // 字体与样式代理
  const std::string &GetMainFont() const { return style_.get_tex_main_font(); }
  const std::string &GetCjkMainFont() const {
    return style_.get_tex_cjk_main_font();
  }
  int GetBaseFontSize() const { return style_.get_base_font_size(); }
  int GetReportTitleFontSize() const {
    return style_.get_report_title_font_size();
  }
  int GetCategoryTitleFontSize() const {
    return style_.get_category_title_font_size();
  }
  double GetMarginIn() const { return style_.get_tex_margin_in(); }
  double GetListTopSepPt() const { return style_.get_tex_list_top_sep_pt(); }
  double GetListItemSepEx() const { return style_.get_tex_list_item_sep_ex(); }

private:
  ExportStyleConfig style_;
  std::string report_title_;
  std::map<std::string, std::string> keyword_colors_;
};

// [修改] 修正模板参数，Config 设为 DayTexConfig
class DayTexFormatter : public BaseTexFormatter<DailyReportData, DayTexConfig> {
public:
  explicit DayTexFormatter(std::shared_ptr<DayTexConfig> config);

protected:
  bool is_empty_data(const DailyReportData &data) const override;
  int get_avg_days(const DailyReportData &data) const override;
  void format_header_content(std::stringstream &ss,
                             const DailyReportData &data) const override;
  void format_extra_content(std::stringstream &ss,
                            const DailyReportData &data) const override;
  std::string get_no_records_msg() const override;
  std::map<std::string, std::string> get_keyword_colors() const override;

private:
  void display_header(std::stringstream &ss, const DailyReportData &data) const;
  void display_detailed_activities(std::stringstream &ss,
                                   const DailyReportData &data) const;
};

#endif
