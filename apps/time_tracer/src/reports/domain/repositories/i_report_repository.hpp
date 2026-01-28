// reports/domain/repositories/i_report_repository.hpp
#ifndef REPORTS_DOMAIN_REPOSITORIES_I_REPORT_REPOSITORY_HPP_
#define REPORTS_DOMAIN_REPOSITORIES_I_REPORT_REPOSITORY_HPP_

#include "reports/domain/model/daily_report_data.hpp"
#include <map>
#include <string>
#include <tuple>
#include <vector>

// RangeReportData 在 Service 层组装，Repo 层不需要知道它，只需要提供基础数据

class IReportRepository {
public:
  virtual ~IReportRepository() = default;

  // --- 单次查询 ---
  virtual DayMetadata get_day_metadata(const std::string &date) = 0;
  virtual std::map<std::string, long long>
  get_day_generated_stats(const std::string &date) = 0;
  virtual std::vector<TimeRecord> get_time_records(const std::string &date) = 0;

  // [核心] Range Report 主要依赖此方法
  virtual std::vector<std::pair<long long, long long>>
  get_aggregated_project_stats(const std::string &start_date,
                               const std::string &end_date) = 0;

  virtual int get_actual_active_days(const std::string &start_date,
                                     const std::string &end_date) = 0;

  // --- 批量导出支持 ---

  // Daily Bulk
  virtual std::map<std::string, DailyReportData> get_all_days_metadata() = 0;
  virtual std::vector<std::pair<std::string, TimeRecord>>
  get_all_time_records_with_date() = 0;

  // Monthly Bulk Optimization
  // 返回 map<"YYYY-MM", vector<pair<projectId, duration>>>
  virtual std::map<std::string, std::vector<std::pair<long long, long long>>>
  get_all_months_project_stats() = 0;

  // 返回 map<"YYYY-MM", actual_days>
  virtual std::map<std::string, int> get_all_months_active_days() = 0;

  // [新增] Weekly Bulk Optimization
  // 返回 map<"YYYY-Www", vector<pair<projectId, duration>>>
  virtual std::map<std::string, std::vector<std::pair<long long, long long>>>
  get_all_weeks_project_stats() = 0;

  // 返回 map<"YYYY-Www", actual_days>
  virtual std::map<std::string, int> get_all_weeks_active_days() = 0;
};

#endif // REPORTS_DOMAIN_REPOSITORIES_I_REPORT_REPOSITORY_HPP_
