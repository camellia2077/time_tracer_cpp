// core/application/service/report_generator.hpp
#ifndef CORE_APPLICATION_SERVICE_REPORT_GENERATOR_HPP_
#define CORE_APPLICATION_SERVICE_REPORT_GENERATOR_HPP_

#include <memory>
#include <string>
#include <vector>
#include "reports/data/model/query_data_structs.hpp"
#include "reports/shared/types/report_format.hpp"
#include "core/domain/repositories/i_report_repository.hpp" // 依赖接口

class ReportGenerator {
public:
    // [修改] 构造函数不再接收 sqlite3* 和 AppConfig，而是接收 Repository 接口
    explicit ReportGenerator(std::shared_ptr<core::domain::repositories::IReportRepository> repository);
    ~ReportGenerator();

    std::string generate_daily_report(const std::string& date, ReportFormat format);
    std::string generate_monthly_report(const std::string& month, ReportFormat format);
    std::string generate_period_report(int days, ReportFormat format);

    FormattedGroupedReports generate_all_daily_reports(ReportFormat format);
    FormattedMonthlyReports generate_all_monthly_reports(ReportFormat format);
    FormattedPeriodReports generate_all_period_reports(const std::vector<int>& days_list, ReportFormat format);

private:
    std::shared_ptr<core::domain::repositories::IReportRepository> repository_;
};

#endif // CORE_APPLICATION_SERVICE_REPORT_GENERATOR_HPP_