// core/infrastructure/persistence/sqlite_report_repository_adapter.hpp
#ifndef CORE_INFRASTRUCTURE_PERSISTENCE_SQLITE_REPORT_REPOSITORY_ADAPTER_HPP_
#define CORE_INFRASTRUCTURE_PERSISTENCE_SQLITE_REPORT_REPOSITORY_ADAPTER_HPP_

#include "common/config/global_report_config.hpp"
#include "core/domain/repositories/i_report_repository.hpp"
#include <memory>

struct sqlite3;
class DailyReportService;
class RangeReportService;

// [修改] 前向声明新的底层数据类
class SqliteReportDataRepository;

namespace infrastructure::persistence {

class SqliteReportRepositoryAdapter
    : public core::domain::repositories::IReportRepository {
public:
  SqliteReportRepositoryAdapter(sqlite3 *db, const GlobalReportConfig &config);
  ~SqliteReportRepositoryAdapter() override;

  std::string GetDailyReport(const std::string &date,
                             ReportFormat format) override;
  std::string GetMonthlyReport(const std::string &month,
                               ReportFormat format) override;
  std::string GetWeeklyReport(int year, int week, ReportFormat format) override;
  std::string GetPeriodReport(int days, ReportFormat format) override;

  FormattedGroupedReports GetAllDailyReports(ReportFormat format) override;

  FormattedWeeklyReports GetAllWeeklyReports(ReportFormat format) override;

  FormattedMonthlyReports GetAllMonthlyReports(ReportFormat format) override;
  FormattedPeriodReports GetAllPeriodReports(const std::vector<int> &days_list,
                                             ReportFormat format) override;

private:
  std::unique_ptr<::SqliteReportDataRepository> data_repo_;
  std::unique_ptr<DailyReportService> daily_service_;
  std::unique_ptr<RangeReportService> range_service_;
};

} // namespace infrastructure::persistence

#endif
