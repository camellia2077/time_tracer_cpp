// reports/presentation/range/common/range_base_config.hpp
#ifndef REPORTS_PRESENTATION_RANGE_COMMON_RANGE_BASE_CONFIG_HPP_
#define REPORTS_PRESENTATION_RANGE_COMMON_RANGE_BASE_CONFIG_HPP_

#include "reports/shared/api/shared_api.hpp"
#include <string>
#include <toml++/toml.hpp>

DISABLE_C4251_WARNING

class REPORTS_SHARED_API RangeBaseConfig {
public:
  explicit RangeBaseConfig(const toml::table &config);
  virtual ~RangeBaseConfig() = default;

  // 报告标题前缀 (可选，例如 "Report: ")
  const std::string &GetReportTitleLabel() const;

  // 日期范围分隔符 (例如 " - " 或 " to ")
  const std::string &GetDateRangeSeparator() const;

  // 统计标签
  const std::string &GetActualDaysLabel() const;
  const std::string &GetTotalTimeLabel() const;

  // 提示信息
  const std::string &GetNoRecordsMessage() const;
  const std::string &GetInvalidDataMessage() const;

  // 章节标题
  const std::string &GetProjectBreakdownLabel() const;

protected:
  toml::table config_table_;

private:
  void load_base_config();

  std::string report_title_label_;
  std::string date_range_separator_;
  std::string actual_days_label_;
  std::string total_time_label_;
  std::string no_records_message_;
  std::string invalid_data_message_;
  std::string project_breakdown_label_;
};

ENABLE_C4251_WARNING

#endif // REPORTS_PRESENTATION_RANGE_COMMON_RANGE_BASE_CONFIG_HPP_
