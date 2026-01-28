// reports/domain/model/daily_report_data.hpp
#ifndef REPORTS_DOMAIN_MODEL_DAILY_REPORT_DATA_HPP_
#define REPORTS_DOMAIN_MODEL_DAILY_REPORT_DATA_HPP_

#include "reports/domain/model/project_tree.hpp"
#include <map>
#include <optional>
#include <string>
#include <vector>

struct TimeRecord {
  std::string start_time_;
  std::string end_time_;
  std::string project_path_;
  long long duration_seconds_ = 0;
  std::optional<std::string> activity_remark_;
};

struct DayMetadata {
  std::string status_ = "N/A";
  std::string sleep_ = "N/A";
  std::string remark_ = "N/A";
  std::string getup_time_ = "N/A";
  std::string exercise_ = "N/A";
};

struct DailyReportData {
  std::string date_;
  DayMetadata metadata_;
  long long total_duration_ = 0;

  // 旧的字符串记录 (为了兼容性暂时保留)
  std::vector<std::pair<std::string, long long>> records_;

  // [新增] 聚合后的统计数据 (ID -> Duration)
  // 这是 BaseQuerier 现在填充的数据源
  std::vector<std::pair<long long, long long>> project_stats_;

  std::vector<TimeRecord> detailed_records_;
  std::map<std::string, long long> stats_;

  // [修改点] 加上命名空间 reporting::
  reporting::ProjectTree project_tree_;
};

#endif // REPORTS_DOMAIN_MODEL_DAILY_REPORT_DATA_HPP_
