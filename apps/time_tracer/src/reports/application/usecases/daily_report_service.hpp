// reports/application/usecases/daily_report_service.hpp
#ifndef REPORTS_APPLICATION_USECASES_DAILY_REPORT_SERVICE_HPP_
#define REPORTS_APPLICATION_USECASES_DAILY_REPORT_SERVICE_HPP_

#include "common/config/global_report_config.hpp" // [修改] 替换 app_config.hpp
#include "core/domain/model/query_data_structs.hpp"
#include "core/domain/types/report_format.hpp"
#include "reports/domain/model/daily_report_data.hpp"
#include "reports/domain/repositories/i_report_repository.hpp"

class DailyReportService {
public:
  // [修改] 依赖 GlobalReportConfig
  explicit DailyReportService(IReportRepository &repo,
                              const GlobalReportConfig &config);

  // [修改] 移除 custom_config_content，直接使用预加载的配置
  std::string generate_report(const std::string &date, ReportFormat format);

  FormattedGroupedReports generate_all_reports(ReportFormat format);

private:
  IReportRepository &repo_;
  const GlobalReportConfig &config_; // [修改] 持有 GlobalReportConfig

  // [新增] 内部辅助：根据格式获取对应的配置对象
  const DailyReportConfig &get_config_by_format(ReportFormat format) const;
};

#endif // REPORTS_APPLICATION_USECASES_DAILY_REPORT_SERVICE_HPP_
