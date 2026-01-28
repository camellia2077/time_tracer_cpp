// reports/presentation/daily/common/day_base_config.cpp
#include "reports/presentation/daily/common/day_base_config.hpp"
#include <stdexcept>

static std::vector<StatisticItemConfig>
parse_statistics_items_recursive(const toml::array *arr) {
  std::vector<StatisticItemConfig> items;
  if (!arr) {
    return items;
  }

  for (const auto &node : *arr) {
    if (!node.is_table())
      continue;
    const auto &item_tbl = *node.as_table();

    StatisticItemConfig item;
    item.label_ = item_tbl["label"].value_or<std::string>("");
    item.db_column_ = item_tbl["db_column"].value_or<std::string>("");
    item.show_ = item_tbl["show"].value_or(true);

    if (const toml::array *sub_arr = item_tbl["sub_items"].as_array()) {
      item.sub_items_ = parse_statistics_items_recursive(sub_arr);
    }

    items.push_back(item);
  }
  return items;
}

// [修改] 构造函数
DayBaseConfig::DayBaseConfig(const toml::table &config)
    : config_table_(config) {
  load_base_config();
}

void DayBaseConfig::load_base_config() {
  // [修改] 使用 TOML++ API
  if (auto val = config_table_["title_prefix"].value<std::string>()) {
    title_prefix_ = *val;
  } else {
    // 兼容 report_title
    title_prefix_ = config_table_["report_title"].value_or("Daily Report for");
  }

  date_label_ = config_table_["date_label"].value_or("");
  total_time_label_ = config_table_["total_time_label"].value_or("");
  status_label_ = config_table_["status_label"].value_or("Status");
  sleep_label_ = config_table_["sleep_label"].value_or("Sleep");
  getup_time_label_ = config_table_["getup_time_label"].value_or("Getup Time");
  remark_label_ = config_table_["remark_label"].value_or("Remark");
  exercise_label_ = config_table_["exercise_label"].value_or("Exercise");

  no_records_ = config_table_["no_records_message"].value_or("No records.");

  statistics_label_ = config_table_["statistics_label"].value_or("Statistics");
  all_activities_label_ =
      config_table_["all_activities_label"].value_or("All Activities");
  activity_remark_label_ =
      config_table_["activity_remark_label"].value_or("Remark");
  activity_connector_ = config_table_["activity_connector"].value_or("->");

  project_breakdown_label_ =
      config_table_["project_breakdown_label"].value_or("Project Breakdown");

  if (const toml::array *arr = config_table_["statistics_items"].as_array()) {
    statistics_items_ = parse_statistics_items_recursive(arr);
  }
}

// Getters 保持不变
const std::string &DayBaseConfig::GetTitlePrefix() const {
  return title_prefix_;
}
const std::string &DayBaseConfig::GetDateLabel() const { return date_label_; }
const std::string &DayBaseConfig::GetTotalTimeLabel() const {
  return total_time_label_;
}
const std::string &DayBaseConfig::GetStatusLabel() const {
  return status_label_;
}
const std::string &DayBaseConfig::GetSleepLabel() const { return sleep_label_; }
const std::string &DayBaseConfig::GetGetupTimeLabel() const {
  return getup_time_label_;
}
const std::string &DayBaseConfig::GetRemarkLabel() const {
  return remark_label_;
}
const std::string &DayBaseConfig::GetExerciseLabel() const {
  return exercise_label_;
}
const std::string &DayBaseConfig::GetNoRecords() const { return no_records_; }
const std::string &DayBaseConfig::GetStatisticsLabel() const {
  return statistics_label_;
}
const std::string &DayBaseConfig::GetAllActivitiesLabel() const {
  return all_activities_label_;
}
const std::string &DayBaseConfig::GetActivityRemarkLabel() const {
  return activity_remark_label_;
}
const std::string &DayBaseConfig::GetActivityConnector() const {
  return activity_connector_;
}
const std::vector<StatisticItemConfig> &
DayBaseConfig::GetStatisticsItems() const {
  return statistics_items_;
}
const std::string &DayBaseConfig::GetProjectBreakdownLabel() const {
  return project_breakdown_label_;
}
