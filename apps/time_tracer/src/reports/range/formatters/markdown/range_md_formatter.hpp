// reports/range/formatters/markdown/range_md_formatter.hpp
#ifndef REPORTS_RANGE_FORMATTERS_MARKDOWN_RANGE_MD_FORMATTER_HPP_
#define REPORTS_RANGE_FORMATTERS_MARKDOWN_RANGE_MD_FORMATTER_HPP_

#include "reports/shared/formatters/templates/base_md_formatter.hpp"
#include "reports/range/formatters/markdown/range_md_config.hpp"
#include "reports/data/model/range_report_data.hpp"

class RangeMdFormatter : public BaseMdFormatter<RangeReportData, RangeMdConfig> {
public:
    explicit RangeMdFormatter(std::shared_ptr<RangeMdConfig> config);

protected:
    // 实现 BaseMdFormatter 要求的钩子函数
    std::string validate_data(const RangeReportData& data) const override;
    bool is_empty_data(const RangeReportData& data) const override;
    int get_avg_days(const RangeReportData& data) const override;
    std::string get_no_records_msg() const override;
    
    // 核心：渲染头部（标题、日期范围、摘要统计）
    void format_header_content(std::stringstream& ss, const RangeReportData& data) const override;
};

#endif // REPORTS_RANGE_FORMATTERS_MARKDOWN_RANGE_MD_FORMATTER_HPP_