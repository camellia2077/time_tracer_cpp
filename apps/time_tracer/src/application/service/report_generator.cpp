// application/service/report_generator.cpp
#include "application/service/report_generator.hpp"

ReportGenerator::ReportGenerator(
    std::shared_ptr<core::domain::repositories::IReportRepository> repository)
    : repository_(std::move(repository)) {}

ReportGenerator::~ReportGenerator() = default;

std::string ReportGenerator::GenerateDailyReport(const std::string &date,
                                                 ReportFormat format) {
  return repository_->GetDailyReport(date, format);
}

std::string ReportGenerator::GenerateWeeklyReport(int year, int week,
                                                  ReportFormat format) {
  return repository_->GetWeeklyReport(year, week, format);
}

std::string ReportGenerator::GenerateMonthlyReport(const std::string &month,
                                                   ReportFormat format) {
  return repository_->GetMonthlyReport(month, format);
}

std::string ReportGenerator::GeneratePeriodReport(int days,
                                                  ReportFormat format) {
  return repository_->GetPeriodReport(days, format);
}

FormattedGroupedReports
ReportGenerator::GenerateAllDailyReports(ReportFormat format) {
  return repository_->GetAllDailyReports(format);
}

FormattedMonthlyReports
ReportGenerator::GenerateAllMonthlyReports(ReportFormat format) {
  return repository_->GetAllMonthlyReports(format);
}

FormattedWeeklyReports
ReportGenerator::GenerateAllWeeklyReports(ReportFormat format) {
  return repository_->GetAllWeeklyReports(format);
}

FormattedPeriodReports
ReportGenerator::GenerateAllPeriodReports(const std::vector<int> &days_list,
                                          ReportFormat format) {
  return repository_->GetAllPeriodReports(days_list, format);
}
