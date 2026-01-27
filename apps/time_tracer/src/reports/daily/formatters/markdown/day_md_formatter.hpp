// reports/daily/formatters/markdown/day_md_formatter.hpp
#ifndef REPORTS_DAILY_FORMATTERS_MARKDOWN_DAY_MD_FORMATTER_HPP_
#define REPORTS_DAILY_FORMATTERS_MARKDOWN_DAY_MD_FORMATTER_HPP_

#include "reports/core/formatters/templates/base_md_formatter.hpp"
#include "reports/data/model/daily_report_data.hpp"
#include "reports/daily/common/day_base_config.hpp" // 直接包含基类配置
#include <toml++/toml.hpp>

// [合并] 将 DayMdConfig 直接定义在此处
// Markdown 不需要复杂的 ExportStyleConfig，直接继承通用配置即可
class DayMdConfig : public DayBaseConfig {
public:
    explicit DayMdConfig(const toml::table& config) : DayBaseConfig(config) {}
};

class DayMdFormatter : public BaseMdFormatter<DailyReportData, DayMdConfig> {
public:
    explicit DayMdFormatter(std::shared_ptr<DayMdConfig> config);

protected:
    // --- 实现基类钩子 ---
    bool is_empty_data(const DailyReportData& data) const override;
    int get_avg_days(const DailyReportData& data) const override;
    
    void format_header_content(std::stringstream& ss, const DailyReportData& data) const override;
    void format_extra_content(std::stringstream& ss, const DailyReportData& data) const override;

    std::string get_no_records_msg() const override;

private:
    void _display_detailed_activities(std::stringstream& ss, const DailyReportData& data) const;
};

#endif // REPORTS_DAILY_FORMATTERS_MARKDOWN_DAY_MD_FORMATTER_HPP_