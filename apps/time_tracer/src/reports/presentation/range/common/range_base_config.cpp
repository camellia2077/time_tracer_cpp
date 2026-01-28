// reports/presentation/range/common/range_base_config.cpp
#include "reports/presentation/range/common/range_base_config.hpp"

RangeBaseConfig::RangeBaseConfig(const toml::table &config)
    : config_table_(config) {
  load_base_config();
}

void RangeBaseConfig::load_base_config() {
  // 使用 toml++ API 获取配置值，提供合理的默认值
  report_title_label_ =
      config_table_["report_title_label"].value_or<std::string>("");
  date_range_separator_ =
      config_table_["date_range_separator"].value_or<std::string>(" - ");

  actual_days_label_ =
      config_table_["actual_days_label"].value_or<std::string>("Active Days");
  total_time_label_ =
      config_table_["total_time_label"].value_or<std::string>("Total Duration");

  no_records_message_ =
      config_table_["no_records_message"].value_or<std::string>(
          "No records found for this range.");
  invalid_data_message_ =
      config_table_["invalid_data_message"].value_or<std::string>(
          "Invalid data range.");

  project_breakdown_label_ =
      config_table_["project_breakdown_label"].value_or<std::string>(
          "Project Breakdown");
}

const std::string &RangeBaseConfig::GetReportTitleLabel() const {
  return report_title_label_;
}
const std::string &RangeBaseConfig::GetDateRangeSeparator() const {
  return date_range_separator_;
}
const std::string &RangeBaseConfig::GetActualDaysLabel() const {
  return actual_days_label_;
}
const std::string &RangeBaseConfig::GetTotalTimeLabel() const {
  return total_time_label_;
}
const std::string &RangeBaseConfig::GetNoRecordsMessage() const {
  return no_records_message_;
}
const std::string &RangeBaseConfig::GetInvalidDataMessage() const {
  return invalid_data_message_;
}
const std::string &RangeBaseConfig::GetProjectBreakdownLabel() const {
  return project_breakdown_label_;
}
