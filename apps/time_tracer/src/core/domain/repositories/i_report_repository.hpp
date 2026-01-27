// core/domain/repositories/i_report_repository.hpp
#ifndef CORE_DOMAIN_REPOSITORIES_I_REPORT_REPOSITORY_HPP_
#define CORE_DOMAIN_REPOSITORIES_I_REPORT_REPOSITORY_HPP_

#include <string>
#include <vector>
#include <map>
#include "reports/core/types/report_format.hpp"
#include "reports/data/model/query_data_structs.hpp"

namespace core::domain::repositories {

class IReportRepository {
public:
    virtual ~IReportRepository() = default;

    virtual std::string get_daily_report(const std::string& date, ReportFormat format) = 0;
    virtual std::string get_monthly_report(const std::string& month, ReportFormat format) = 0;
    virtual std::string get_weekly_report(int year, int week, ReportFormat format) = 0;
    virtual std::string get_period_report(int days, ReportFormat format) = 0;

    virtual FormattedGroupedReports get_all_daily_reports(ReportFormat format) = 0;
    virtual FormattedMonthlyReports get_all_monthly_reports(ReportFormat format) = 0;
    
    virtual FormattedWeeklyReports get_all_weekly_reports(ReportFormat format) = 0;
    
    virtual FormattedPeriodReports get_all_period_reports(const std::vector<int>& days_list, ReportFormat format) = 0;
};

} // namespace core::domain::repositories

#endif