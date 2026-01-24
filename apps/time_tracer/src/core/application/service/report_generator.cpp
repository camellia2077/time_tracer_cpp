// core/application/service/report_generator.cpp
#include "core/application/service/report_generator.hpp"

ReportGenerator::ReportGenerator(std::shared_ptr<core::domain::repositories::IReportRepository> repository)
    : repository_(std::move(repository)) {}

ReportGenerator::~ReportGenerator() = default;

std::string ReportGenerator::generate_daily_report(const std::string& date, ReportFormat format) {
    return repository_->get_daily_report(date, format);
}

std::string ReportGenerator::generate_monthly_report(const std::string& month, ReportFormat format) {
    return repository_->get_monthly_report(month, format);
}

std::string ReportGenerator::generate_period_report(int days, ReportFormat format) {
    return repository_->get_period_report(days, format);
}

FormattedGroupedReports ReportGenerator::generate_all_daily_reports(ReportFormat format) {
    return repository_->get_all_daily_reports(format);
}

FormattedMonthlyReports ReportGenerator::generate_all_monthly_reports(ReportFormat format) {
    return repository_->get_all_monthly_reports(format);
}

FormattedPeriodReports ReportGenerator::generate_all_period_reports(const std::vector<int>& days_list, ReportFormat format) {
    return repository_->get_all_period_reports(days_list, format);
}