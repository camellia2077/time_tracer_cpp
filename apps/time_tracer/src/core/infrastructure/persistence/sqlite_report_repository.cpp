// core/infrastructure/persistence/sqlite_report_repository.cpp
#include "core/infrastructure/persistence/sqlite_report_repository.hpp"
#include "reports/report_service.hpp" // 包含实际的业务逻辑实现

namespace infrastructure::persistence {

SqliteReportRepository::SqliteReportRepository(sqlite3* db, const AppConfig& config) {
    // 这里的 ReportService 是旧有的实现，现在被封装在 Infrastructure 层内部
    report_service_ = std::make_unique<ReportService>(db, config);
}

SqliteReportRepository::~SqliteReportRepository() = default;

std::string SqliteReportRepository::get_daily_report(const std::string& date, ReportFormat format) {
    return report_service_->run_daily_query(date, format);
}

std::string SqliteReportRepository::get_monthly_report(const std::string& month, ReportFormat format) {
    return report_service_->run_monthly_query(month, format);
}

std::string SqliteReportRepository::get_period_report(int days, ReportFormat format) {
    return report_service_->run_period_query(days, format);
}

FormattedGroupedReports SqliteReportRepository::get_all_daily_reports(ReportFormat format) {
    return report_service_->run_export_all_daily_reports_query(format);
}

FormattedMonthlyReports SqliteReportRepository::get_all_monthly_reports(ReportFormat format) {
    return report_service_->run_export_all_monthly_reports_query(format);
}

FormattedPeriodReports SqliteReportRepository::get_all_period_reports(const std::vector<int>& days_list, ReportFormat format) {
    return report_service_->run_export_all_period_reports_query(days_list, format);
}

} // namespace infrastructure::persistence