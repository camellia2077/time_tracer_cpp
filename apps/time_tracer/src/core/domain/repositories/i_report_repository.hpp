// core/domain/repositories/i_report_repository.hpp
#ifndef CORE_DOMAIN_REPOSITORIES_I_REPORT_REPOSITORY_HPP_
#define CORE_DOMAIN_REPOSITORIES_I_REPORT_REPOSITORY_HPP_

#include "core/domain/model/query_data_structs.hpp"
#include "core/domain/types/report_format.hpp"
#include <map>
#include <string>
#include <vector>

namespace core::domain::repositories {

class IReportRepository {
public:
  virtual ~IReportRepository() = default;

  virtual std::string GetDailyReport(const std::string &date,
                                     ReportFormat format) = 0;
  virtual std::string GetMonthlyReport(const std::string &month,
                                       ReportFormat format) = 0;
  virtual std::string GetWeeklyReport(int year, int week,
                                      ReportFormat format) = 0;
  virtual std::string GetPeriodReport(int days, ReportFormat format) = 0;

  virtual FormattedGroupedReports GetAllDailyReports(ReportFormat format) = 0;
  virtual FormattedMonthlyReports GetAllMonthlyReports(ReportFormat format) = 0;

  virtual FormattedWeeklyReports GetAllWeeklyReports(ReportFormat format) = 0;

  virtual FormattedPeriodReports
  GetAllPeriodReports(const std::vector<int> &days_list,
                      ReportFormat format) = 0;
};

} // namespace core::domain::repositories

#endif
