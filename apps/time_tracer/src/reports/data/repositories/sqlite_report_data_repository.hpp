// reports/data/repositories/sqlite_report_data_repository.hpp
#ifndef REPORTS_DATA_REPOSITORIES_SQLITE_REPORT_DATA_REPOSITORY_HPP_
#define REPORTS_DATA_REPOSITORIES_SQLITE_REPORT_DATA_REPOSITORY_HPP_

#include "reports/data/repositories/i_report_repository.hpp"
#include <sqlite3.h>

class SqliteReportDataRepository : public IReportRepository {
public:
    explicit SqliteReportDataRepository(sqlite3* db);

    // Single Query Methods
    DayMetadata get_day_metadata(const std::string& date) override;
    std::map<std::string, long long> get_day_generated_stats(const std::string& date) override;
    std::vector<TimeRecord> get_time_records(const std::string& date) override;
    std::vector<std::pair<long long, long long>> get_aggregated_project_stats(const std::string& start, const std::string& end) override;
    int get_actual_active_days(const std::string& start, const std::string& end) override;

    // Bulk Export Methods
    std::map<std::string, DailyReportData> get_all_days_metadata() override;
    std::vector<std::pair<std::string, TimeRecord>> get_all_time_records_with_date() override;
    std::map<std::string, std::vector<std::pair<long long, long long>>> get_all_months_project_stats() override;
    std::map<std::string, int> get_all_months_active_days() override;

    // [新增] Weekly Bulk Implementation
    std::map<std::string, std::vector<std::pair<long long, long long>>> get_all_weeks_project_stats() override;
    std::map<std::string, int> get_all_weeks_active_days() override;

private:
    sqlite3* db_;
};

#endif // REPORTS_DATA_REPOSITORIES_SQLITE_REPORT_DATA_REPOSITORY_HPP_