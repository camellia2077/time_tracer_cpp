// reports/application/usecases/range_report_service.hpp
#ifndef REPORTS_APPLICATION_USECASES_RANGE_REPORT_SERVICE_HPP_
#define REPORTS_APPLICATION_USECASES_RANGE_REPORT_SERVICE_HPP_

#include "common/config/global_report_config.hpp"
#include "core/domain/types/report_format.hpp"
#include "reports/domain/model/range_report_data.hpp"
#include "reports/domain/repositories/i_report_repository.hpp"
#include <map>
#include <string>
#include <vector>

// [新增] 定义范围类型，用于选择对应的配置 (Week/Month/Period)
enum class RangeType {
  Period, // Default generic range
  Month,
  Week
};

struct RangeRequest {
  std::string name;
  std::string start_date;
  std::string end_date;
  int covered_days;
  // [新增]
  RangeType type = RangeType::Period;
};

class RangeReportService {
public:
  explicit RangeReportService(IReportRepository &repo,
                              const GlobalReportConfig &config);

  std::string generate_report(const RangeRequest &request, ReportFormat format);

  std::map<std::string, std::string>
  generate_batch(const std::vector<RangeRequest> &requests,
                 ReportFormat format);
  std::map<int, std::map<int, std::string>>
  generate_all_weekly_history(ReportFormat format); // 生成所有历史周报
  std::map<int, std::map<int, std::string>>
  generate_all_monthly_history(ReportFormat format); // 生成所有历史月报

private:
  IReportRepository &repo_;
  const GlobalReportConfig &config_;

  RangeReportData build_data_for_range(const RangeRequest &request);

  // [修改] 增加 RangeType 参数
  const RangeReportConfig &get_config_by_format(ReportFormat format,
                                                RangeType type) const;
};

#endif // REPORTS_APPLICATION_USECASES_RANGE_REPORT_SERVICE_HPP_
