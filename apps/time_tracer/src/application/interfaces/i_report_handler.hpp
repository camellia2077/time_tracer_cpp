// application/interfaces/i_report_handler.hpp
#ifndef APPLICATION_INTERFACES_I_REPORT_HANDLER_HPP_
#define APPLICATION_INTERFACES_I_REPORT_HANDLER_HPP_

#include "core/domain/types/report_format.hpp"
#include <string>
#include <vector>

class IReportHandler {
public:
  virtual ~IReportHandler() = default;

  // 查询方法
  virtual std::string RunDailyQuery(const std::string &date,
                                    ReportFormat format) = 0;
  virtual std::string RunWeeklyQuery(int year, int week,
                                     ReportFormat format) = 0;
  virtual std::string RunMonthlyQuery(const std::string &month,
                                      ReportFormat format) = 0;
  virtual std::string RunPeriodQuery(int days, ReportFormat format) = 0;

  // [新增] 批量周期查询接口
  virtual std::string RunPeriodQueries(const std::vector<int> &days_list,
                                       ReportFormat format) = 0;

  // 导出方法
  virtual void RunExportSingleDayReport(const std::string &date,
                                        ReportFormat format) = 0;
  virtual void RunExportSingleMonthReport(const std::string &month,
                                          ReportFormat format) = 0;
  virtual void RunExportSinglePeriodReport(int days, ReportFormat format) = 0;
  virtual void RunExportSingleWeekReport(int year, int week,
                                         ReportFormat format) = 0;
  virtual void RunExportAllDailyReportsQuery(ReportFormat format) = 0;
  virtual void RunExportAllWeeklyReportsQuery(ReportFormat format) = 0;
  virtual void RunExportAllMonthlyReportsQuery(ReportFormat format) = 0;
  virtual void RunExportAllPeriodReportsQuery(const std::vector<int> &days_list,
                                              ReportFormat format) = 0;
};

#endif // APPLICATION_INTERFACES_I_REPORT_HANDLER_HPP_
