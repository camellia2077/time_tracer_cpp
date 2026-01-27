// reports/daily/formatters/latex/day_tex_formatter.hpp
#ifndef REPORTS_DAILY_FORMATTERS_LATEX_DAY_TEX_FORMATTER_HPP_
#define REPORTS_DAILY_FORMATTERS_LATEX_DAY_TEX_FORMATTER_HPP_

#include "reports/core/formatters/templates/base_tex_formatter.hpp"
#include "reports/data/model/daily_report_data.hpp"
#include "reports/daily/common/day_base_config.hpp"
#include "reports/core/config/export_style_config.hpp"
#include <toml++/toml.hpp>
#include <map>
#include <string>

// [合并] 将配置类定义移入此处
class DayTexConfig : public DayBaseConfig {
public:
    explicit DayTexConfig(const toml::table& config);

    const std::string& get_report_title() const { return report_title_; }
    const std::map<std::string, std::string>& get_keyword_colors() const { return keyword_colors_; }

    // 字体与样式代理
    const std::string& get_main_font() const { return style_.get_tex_main_font(); }
    const std::string& get_cjk_main_font() const { return style_.get_tex_cjk_main_font(); }
    int get_base_font_size() const { return style_.get_base_font_size(); }
    int get_report_title_font_size() const { return style_.get_report_title_font_size(); }
    int get_category_title_font_size() const { return style_.get_category_title_font_size(); }
    double get_margin_in() const { return style_.get_tex_margin_in(); }
    double get_list_top_sep_pt() const { return style_.get_tex_list_top_sep_pt(); }
    double get_list_item_sep_ex() const { return style_.get_tex_list_item_sep_ex(); }

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
    bool is_empty_data(const DailyReportData& data) const override;
    int get_avg_days(const DailyReportData& data) const override;
    void format_header_content(std::stringstream& ss, const DailyReportData& data) const override;
    void format_extra_content(std::stringstream& ss, const DailyReportData& data) const override;
    std::string get_no_records_msg() const override;
    std::map<std::string, std::string> get_keyword_colors() const override;

private:
    void display_header(std::stringstream& ss, const DailyReportData& data) const;
    void display_detailed_activities(std::stringstream& ss, const DailyReportData& data) const;
};

#endif