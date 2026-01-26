// reports/report_service.hpp
#ifndef REPORTS_REPORT_SERVICE_HPP_
#define REPORTS_REPORT_SERVICE_HPP_

#include <sqlite3.h>
#include <string>
#include <vector>
#include <memory>
#include "reports/data/model/query_data_structs.hpp"
#include "reports/shared/types/report_format.hpp" 
#include "common/config/app_config.hpp"

// [新增] 前置声明
class IReportRepository;

class ReportService {
public:
    explicit ReportService(sqlite3* db, const AppConfig& config);
    ~ReportService();

    // --- Single Queries ---
    std::string run_daily_query(const std::string& date_str, ReportFormat format) const;
    
    // [保留接口] 内部改为调用 RangeReportService
    std::string run_period_query(int days, ReportFormat format) const;
    std::string run_monthly_query(const std::string& year_month_str, ReportFormat format) const;

    // --- Bulk Export Queries ---
    FormattedGroupedReports run_export_all_daily_reports_query(ReportFormat format) const;
    
    // [保留接口] 内部适配 Range 逻辑
    FormattedMonthlyReports run_export_all_monthly_reports_query(ReportFormat format) const;
    FormattedPeriodReports run_export_all_period_reports_query(const std::vector<int>& days_list, ReportFormat format) const;

private:
    std::unique_ptr<IReportRepository> repo_;
    const AppConfig& app_config_;
};

#endif // REPORTS_REPORT_SERVICE_HPP_