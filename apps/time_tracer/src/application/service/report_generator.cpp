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

std::string ReportGenerator::GenerateYearlyReport(int year,
                                                  ReportFormat format) {
  return repository_->GetYearlyReport(year, format);
}

std::string ReportGenerator::GenerateMonthlyReport(const std::string &month,
                                                   ReportFormat format) {
  return repository_->GetMonthlyReport(month, format);
}

std::string ReportGenerator::GenerateRecentReport(int days,
                                                  ReportFormat format) {
  return repository_->GetRecentReport(days, format);
}

std::string ReportGenerator::GenerateRangeReport(const std::string &start_date,
                                                 const std::string &end_date,
                                                 ReportFormat format) {
  return repository_->GetRangeReport(start_date, end_date, format);
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

FormattedRecentReports
ReportGenerator::GenerateAllRecentReports(const std::vector<int> &days_list,
                                          ReportFormat format) {
  return repository_->GetAllRecentReports(days_list, format);
}

FormattedYearlyReports
ReportGenerator::GenerateAllYearlyReports(ReportFormat format) {
  return repository_->GetAllYearlyReports(format);
}
