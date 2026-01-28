// application/service/report_generator.hpp
#ifndef APPLICATION_SERVICE_REPORT_GENERATOR_HPP_
#define APPLICATION_SERVICE_REPORT_GENERATOR_HPP_

#include "core/domain/model/query_data_structs.hpp"
#include "core/domain/repositories/i_report_repository.hpp"
#include "core/domain/types/report_format.hpp"
#include <memory>
#include <string>
#include <vector>

class ReportGenerator {
public:
  explicit ReportGenerator(
      std::shared_ptr<core::domain::repositories::IReportRepository>
          repository);
  ~ReportGenerator();

  std::string GenerateDailyReport(const std::string &date, ReportFormat format);
  std::string GenerateWeeklyReport(int year, int week, ReportFormat format);
  std::string GenerateMonthlyReport(const std::string &month,
                                    ReportFormat format);
  std::string GeneratePeriodReport(int days, ReportFormat format);

  FormattedGroupedReports GenerateAllDailyReports(ReportFormat format);
  FormattedMonthlyReports GenerateAllMonthlyReports(ReportFormat format);

  FormattedWeeklyReports GenerateAllWeeklyReports(ReportFormat format);

  FormattedPeriodReports
  GenerateAllPeriodReports(const std::vector<int> &days_list,
                           ReportFormat format);

private:
  std::shared_ptr<core::domain::repositories::IReportRepository> repository_;
};

#endif
