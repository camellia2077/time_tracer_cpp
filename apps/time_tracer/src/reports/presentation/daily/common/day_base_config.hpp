// reports/presentation/daily/common/day_base_config.hpp
#ifndef REPORTS_PRESENTATION_DAILY_COMMON_DAY_BASE_CONFIG_HPP_
#define REPORTS_PRESENTATION_DAILY_COMMON_DAY_BASE_CONFIG_HPP_

#include "reports/shared/api/shared_api.hpp"
#include <map>
#include <string>
#include <toml++/toml.hpp>
#include <vector>

struct StatisticItemConfig {
  std::string label_;
  std::string db_column_;
  bool show_ = true;
  std::vector<StatisticItemConfig> sub_items_;
};

DISABLE_C4251_WARNING

class REPORTS_SHARED_API DayBaseConfig {
public:
  // [修改] 构造函数接收 TOML Table
  explicit DayBaseConfig(const toml::table &config);
  virtual ~DayBaseConfig() = default;

  const std::string &GetTitlePrefix() const;
  const std::string &GetDateLabel() const;
  const std::string &GetTotalTimeLabel() const;
  const std::string &GetStatusLabel() const;
  const std::string &GetSleepLabel() const;
  const std::string &GetGetupTimeLabel() const;
  const std::string &GetRemarkLabel() const;
  const std::string &GetExerciseLabel() const;
  const std::string &GetNoRecords() const;
  const std::string &GetStatisticsLabel() const;
  const std::string &GetAllActivitiesLabel() const;
  const std::string &GetActivityRemarkLabel() const;
  const std::string &GetActivityConnector() const;
  const std::string &GetProjectBreakdownLabel() const;

  const std::vector<StatisticItemConfig> &GetStatisticsItems() const;

protected:
  toml::table config_table_;

private:
  void load_base_config();

  std::string title_prefix_;
  std::string date_label_;
  std::string total_time_label_;
  std::string status_label_;
  std::string sleep_label_;
  std::string getup_time_label_;
  std::string remark_label_;
  std::string exercise_label_;
  std::string no_records_;
  std::string statistics_label_;
  std::string all_activities_label_;
  std::string activity_remark_label_;
  std::string activity_connector_;
  std::string project_breakdown_label_;

  std::vector<StatisticItemConfig> statistics_items_;
};

ENABLE_C4251_WARNING

#endif // REPORTS_PRESENTATION_DAILY_COMMON_DAY_BASE_CONFIG_HPP_
