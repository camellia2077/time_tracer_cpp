// reports/daily/formatters/typst/day_typ_formatter.hpp
#ifndef REPORTS_DAILY_FORMATTERS_TYPST_DAY_TYP_FORMATTER_HPP_
#define REPORTS_DAILY_FORMATTERS_TYPST_DAY_TYP_FORMATTER_HPP_

#include "reports/core/formatters/templates/base_typ_formatter.hpp"
#include "reports/data/model/daily_report_data.hpp"
#include "reports/daily/common/day_base_config.hpp"
#include "reports/core/config/export_style_config.hpp"
#include <toml++/toml.hpp>
#include <map>
#include <string>
#include <memory>

// [合并] 配置类定义
class DayTypConfig : public DayBaseConfig {
public:
    explicit DayTypConfig(const toml::table& config);

    int get_statistic_font_size() const { return statistic_font_size_; }
    int get_statistic_title_font_size() const { return statistic_title_font_size_; }
    const std::map<std::string, std::string>& get_keyword_colors() const { return keyword_colors_; }
    
    const std::string& get_title_font() const { return style_.get_typst_title_font(); }
    const std::string& get_base_font() const { return style_.get_typst_base_font(); }
    const std::string& get_category_title_font() const { return style_.get_typst_category_title_font(); }
    int get_base_font_size() const { return style_.get_base_font_size(); }
    int get_report_title_font_size() const { return style_.get_report_title_font_size(); }
    int get_category_title_font_size() const { return style_.get_category_title_font_size(); }
    double get_line_spacing_em() const { return style_.get_typst_line_spacing_em(); }

private:
    ExportStyleConfig style_;
    int statistic_font_size_;
    int statistic_title_font_size_;
    std::map<std::string, std::string> keyword_colors_;
};

// [修改] 修正模板参数，Config 设为 DayTypConfig
class DayTypFormatter : public BaseTypFormatter<DailyReportData, DayTypConfig> {
public:
    explicit DayTypFormatter(std::shared_ptr<DayTypConfig> config);

protected:
    bool is_empty_data(const DailyReportData& data) const override;
    int get_avg_days(const DailyReportData& data) const override;
    void format_header_content(std::stringstream& ss, const DailyReportData& data) const override;
    void format_extra_content(std::stringstream& ss, const DailyReportData& data) const override;
    std::string get_no_records_msg() const override;

private:
    void display_header(std::stringstream& ss, const DailyReportData& data) const;
    void display_detailed_activities(std::stringstream& ss, const DailyReportData& data) const;
    std::string format_activity_line(const TimeRecord& record) const;
};

#endif