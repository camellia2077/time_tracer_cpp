// reports/services/range_report_service.hpp
#ifndef REPORTS_SERVICES_RANGE_REPORT_SERVICE_HPP_
#define REPORTS_SERVICES_RANGE_REPORT_SERVICE_HPP_

#include <vector>
#include <string>
#include <map>
#include "reports/data/repositories/i_report_repository.hpp"
#include "reports/data/model/range_report_data.hpp"
#include "common/config/app_config.hpp"
#include "reports/shared/types/report_format.hpp"

// 请求参数：由调用者定义时间范围
struct RangeRequest {
    std::string name;       // e.g. "Last 7 Days", "2025-01"
    std::string start_date; // YYYY-MM-DD
    std::string end_date;   // YYYY-MM-DD
    int covered_days;       // 用于计算平均值的分母
};

class RangeReportService {
public:
    explicit RangeReportService(IReportRepository& repo, const AppConfig& config);

    /**
     * @brief 生成单个时间范围的报告
     */
    std::string generate_report(const RangeRequest& request, ReportFormat format);

    /**
     * @brief 批量生成多个时间范围的报告
     * @return map<report_name, formatted_content>
     */
    std::map<std::string, std::string> generate_batch(
        const std::vector<RangeRequest>& requests, 
        ReportFormat format
    );

    /**
     * @brief 导出所有历史月份的报告 (性能优化版)
     * 利用 Repository 的批量聚合接口，避免 N 次查询
     * @return map<Year, map<Month, Content>>
     */
    std::map<int, std::map<int, std::string>> generate_all_monthly_history(ReportFormat format);

private:
    IReportRepository& repo_;
    const AppConfig& app_config_;
    
    // 内部核心逻辑：构建数据对象
    RangeReportData build_data_for_range(const RangeRequest& request);
};

#endif // REPORTS_SERVICES_RANGE_REPORT_SERVICE_HPP_