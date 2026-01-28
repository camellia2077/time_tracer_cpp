// reports/presentation/range/formatters/markdown/range_md_formatter.hpp
#ifndef REPORTS_PRESENTATION_RANGE_FORMATTERS_MARKDOWN_RANGE_MD_FORMATTER_HPP_
#define REPORTS_PRESENTATION_RANGE_FORMATTERS_MARKDOWN_RANGE_MD_FORMATTER_HPP_

#include "reports/domain/model/range_report_data.hpp"
#include "reports/presentation/range/common/range_base_config.hpp"
#include "reports/shared/formatters/templates/base_md_formatter.hpp"
#include <memory>
#include <string>
#include <toml++/toml.hpp>

/**
 * @class RangeMdFormatterConfig
 * @brief 范围报表 Markdown 专用的本地配置类，避免与全局别名冲突。
 */
class RangeMdFormatterConfig : public RangeBaseConfig {
public:
  explicit RangeMdFormatterConfig(const toml::table &config)
      : RangeBaseConfig(config) {}
};

/**
 * @class RangeMdFormatter
 * @brief 负责范围报表（周报/月报等）的 Markdown 格式化。
 */
class RangeMdFormatter
    : public BaseMdFormatter<RangeReportData, RangeMdFormatterConfig> {
public:
  explicit RangeMdFormatter(std::shared_ptr<RangeMdFormatterConfig> config);

protected:
  // --- 实现 BaseMdFormatter 钩子 ---
  std::string validate_data(const RangeReportData &data) const override;
  bool is_empty_data(const RangeReportData &data) const override;
  int get_avg_days(const RangeReportData &data) const override;
  std::string get_no_records_msg() const override;

  // 渲染头部（标题、日期范围、摘要统计）
  void format_header_content(std::stringstream &ss,
                             const RangeReportData &data) const override;
};

#endif // REPORTS_PRESENTATION_RANGE_FORMATTERS_MARKDOWN_RANGE_MD_FORMATTER_HPP_
