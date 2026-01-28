// application/service/report_handler.hpp
#ifndef APPLICATION_SERVICE_REPORT_HANDLER_HPP_
#define APPLICATION_SERVICE_REPORT_HANDLER_HPP_

#include "application/interfaces/i_report_handler.hpp"
#include <memory>
#include <string> // [新增]

class ReportGenerator;
class Exporter;

class ReportHandler : public IReportHandler {
public:
  // [修改] 增加 app_root_dir 参数
  ReportHandler(std::unique_ptr<ReportGenerator> generator,
                std::unique_ptr<Exporter> exporter,
                const std::string &app_root_dir);

  ~ReportHandler() override;

  std::string RunDailyQuery(const std::string &date,
                            ReportFormat format) override;
  std::string RunMonthlyQuery(const std::string &month,
                              ReportFormat format) override;
  std::string RunWeeklyQuery(int year, int week, ReportFormat format) override;
  std::string RunYearlyQuery(int year, ReportFormat format) override;
  std::string RunRecentQuery(int days, ReportFormat format) override;
  std::string RunRecentQueries(const std::vector<int> &days_list,
                               ReportFormat format) override;
  std::string RunRangeQuery(const std::string &start_date,
                            const std::string &end_date,
                            ReportFormat format) override;

  void RunExportSingleDayReport(const std::string &date,
                                ReportFormat format) override;
  void RunExportSingleMonthReport(const std::string &month,
                                  ReportFormat format) override;
  void RunExportSingleWeekReport(int year, int week,
                                 ReportFormat format) override;
  void RunExportSingleYearReport(int year, ReportFormat format) override;
  void RunExportSingleRecentReport(int days, ReportFormat format) override;
  void RunExportSingleRangeReport(const std::string &start_date,
                                  const std::string &end_date,
                                  ReportFormat format) override;

  void RunExportAllDailyReportsQuery(ReportFormat format) override;
  void RunExportAllWeeklyReportsQuery(ReportFormat format) override;
  void RunExportAllMonthlyReportsQuery(ReportFormat format) override;
  void RunExportAllYearlyReportsQuery(ReportFormat format) override;
  void RunExportAllRecentReportsQuery(const std::vector<int> &days_list,
                                      ReportFormat format) override;

private:
  std::unique_ptr<ReportGenerator> generator_;
  std::unique_ptr<Exporter> exporter_;
};

#endif
