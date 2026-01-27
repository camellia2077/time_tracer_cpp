// core/application/service/report_handler.hpp
#ifndef CORE_APPLICATION_SERVICE_REPORT_HANDLER_HPP_
#define CORE_APPLICATION_SERVICE_REPORT_HANDLER_HPP_

#include "core/application/interfaces/i_report_handler.hpp"
#include <memory>
#include <string> // [新增]

class ReportGenerator;
class Exporter;

class ReportHandler : public IReportHandler {
public:
    // [修改] 增加 app_root_dir 参数
    ReportHandler(std::unique_ptr<ReportGenerator> generator, 
                  std::unique_ptr<Exporter> exporter,
                  const std::string& app_root_dir);
    
    ~ReportHandler() override;

    std::string run_daily_query(const std::string& date, ReportFormat format) override;
    std::string run_monthly_query(const std::string& month, ReportFormat format) override;
    std::string run_weekly_query(int year, int week, ReportFormat format) override;
    std::string run_period_query(int days, ReportFormat format) override;
    std::string run_period_queries(const std::vector<int>& days_list, ReportFormat format) override;

    void run_export_single_day_report(const std::string& date, ReportFormat format) override;
    void run_export_single_month_report(const std::string& month, ReportFormat format) override;
    void run_export_single_period_report(int days, ReportFormat format) override;
    void run_export_single_week_report(int year, int week, ReportFormat format) override;
    
    void run_export_all_daily_reports_query(ReportFormat format) override;
    void run_export_all_weekly_reports_query(ReportFormat format) override;
    void run_export_all_monthly_reports_query(ReportFormat format) override;
    void run_export_all_period_reports_query(const std::vector<int>& days_list, ReportFormat format) override;

private:
    std::unique_ptr<ReportGenerator> generator_;
    std::unique_ptr<Exporter> exporter_;
};

#endif