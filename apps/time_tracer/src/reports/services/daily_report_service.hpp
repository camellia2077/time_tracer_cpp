// reports/services/daily_report_service.hpp
#ifndef REPORTS_SERVICES_DAILY_REPORT_SERVICE_HPP_
#define REPORTS_SERVICES_DAILY_REPORT_SERVICE_HPP_ 

#include "reports/data/model/daily_report_data.hpp"
#include "reports/shared/types/report_format.hpp" 
#include "common/config/app_config.hpp"
#include "reports/data/repositories/i_report_repository.hpp"

class DailyReportService {
public:
    explicit DailyReportService(IReportRepository& repo, const AppConfig& config);

    // [新增] 单日报告生成接口
    std::string generate_report(const std::string& date, ReportFormat format);

    FormattedGroupedReports generate_all_reports(ReportFormat format);

private:
    IReportRepository& repo_;
    const AppConfig& app_config_;
};

#endif