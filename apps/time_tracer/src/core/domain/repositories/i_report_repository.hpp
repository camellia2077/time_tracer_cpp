// core/domain/repositories/i_report_repository.hpp
#ifndef CORE_DOMAIN_REPOSITORIES_I_REPORT_REPOSITORY_HPP_
#define CORE_DOMAIN_REPOSITORIES_I_REPORT_REPOSITORY_HPP_

#include <string>
#include <vector>
#include "reports/shared/types/report_format.hpp"
#include "reports/data/model/query_data_structs.hpp"

namespace core::domain::repositories {

/**
 * @brief 报表数据仓库接口
 * * 定义获取报表数据的方法，屏蔽底层数据库实现 (SQLite/PostgreSQL等)
 */
class IReportRepository {
public:
    virtual ~IReportRepository() = default;

    // 单个查询 (目前返回格式化后的字符串，理想情况下应返回 DTO 由 Domain 层格式化，
    // 但为了兼容现有 ReportService 逻辑，暂保持返回 string)
    virtual std::string get_daily_report(const std::string& date, ReportFormat format) = 0;
    virtual std::string get_monthly_report(const std::string& month, ReportFormat format) = 0;
    virtual std::string get_period_report(int days, ReportFormat format) = 0;

    // 批量查询 (返回结构化数据)
    virtual FormattedGroupedReports get_all_daily_reports(ReportFormat format) = 0;
    virtual FormattedMonthlyReports get_all_monthly_reports(ReportFormat format) = 0;
    virtual FormattedPeriodReports get_all_period_reports(const std::vector<int>& days_list, ReportFormat format) = 0;
};

} // namespace core::domain::repositories

#endif // CORE_DOMAIN_REPOSITORIES_I_REPORT_REPOSITORY_HPP_