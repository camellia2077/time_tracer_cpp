// core/infrastructure/persistence/sqlite_report_repository.hpp
#ifndef CORE_INFRASTRUCTURE_PERSISTENCE_SQLITE_REPORT_REPOSITORY_HPP_
#define CORE_INFRASTRUCTURE_PERSISTENCE_SQLITE_REPORT_REPOSITORY_HPP_

#include "core/domain/repositories/i_report_repository.hpp"
#include "common/config/app_config.hpp"
#include <memory>

struct sqlite3;
class ReportService; // 前向声明原有的服务类

namespace infrastructure::persistence {

class SqliteReportRepository : public core::domain::repositories::IReportRepository {
public:
    SqliteReportRepository(sqlite3* db, const AppConfig& config);
    ~SqliteReportRepository() override;

    std::string get_daily_report(const std::string& date, ReportFormat format) override;
    std::string get_monthly_report(const std::string& month, ReportFormat format) override;
    std::string get_period_report(int days, ReportFormat format) override;

    FormattedGroupedReports get_all_daily_reports(ReportFormat format) override;
    FormattedMonthlyReports get_all_monthly_reports(ReportFormat format) override;
    FormattedPeriodReports get_all_period_reports(const std::vector<int>& days_list, ReportFormat format) override;

private:
    std::unique_ptr<ReportService> report_service_;
};

} // namespace infrastructure::persistence

#endif // CORE_INFRASTRUCTURE_PERSISTENCE_SQLITE_REPORT_REPOSITORY_HPP_