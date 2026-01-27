// core/application/service/report_generator.hpp
#ifndef CORE_APPLICATION_SERVICE_REPORT_GENERATOR_HPP_
#define CORE_APPLICATION_SERVICE_REPORT_GENERATOR_HPP_

#include <memory>
#include <string>
#include <vector>
#include "reports/data/model/query_data_structs.hpp"
#include "reports/shared/types/report_format.hpp"
#include "core/domain/repositories/i_report_repository.hpp"

class ReportGenerator {
public:
    explicit ReportGenerator(std::shared_ptr<core::domain::repositories::IReportRepository> repository);
    ~ReportGenerator();

    std::string generate_daily_report(const std::string& date, ReportFormat format);
    std::string generate_weekly_report(int year, int week, ReportFormat format);
    std::string generate_monthly_report(const std::string& month, ReportFormat format);
    std::string generate_period_report(int days, ReportFormat format);

    FormattedGroupedReports generate_all_daily_reports(ReportFormat format);
    FormattedMonthlyReports generate_all_monthly_reports(ReportFormat format);
    
    FormattedWeeklyReports generate_all_weekly_reports(ReportFormat format);
    
    FormattedPeriodReports generate_all_period_reports(const std::vector<int>& days_list, ReportFormat format);

private:
    std::shared_ptr<core::domain::repositories::IReportRepository> repository_;
};

#endif