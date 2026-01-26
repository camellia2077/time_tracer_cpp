// reports/data/repositories/sqlite_report_repository.cpp
#include "reports/data/repositories/sqlite_report_repository.hpp"
#include <stdexcept>
#include <iostream>
#include <vector>

SqliteReportRepository::SqliteReportRepository(sqlite3* db) : db_(db) {
    if (!db_) throw std::invalid_argument("Database connection cannot be null.");
}

// --- Single Query Methods ---

DayMetadata SqliteReportRepository::get_day_metadata(const std::string& date) {
    DayMetadata metadata;
    sqlite3_stmt* stmt;
    std::string sql = "SELECT status, sleep, remark, getup_time, exercise FROM days WHERE date = ?;";
    
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            metadata.status = std::to_string(sqlite3_column_int(stmt, 0));
            metadata.sleep = std::to_string(sqlite3_column_int(stmt, 1));
            const unsigned char* r = sqlite3_column_text(stmt, 2);
            if (r) metadata.remark = reinterpret_cast<const char*>(r);
            const unsigned char* g = sqlite3_column_text(stmt, 3);
            if (g) metadata.getup_time = reinterpret_cast<const char*>(g);
            metadata.exercise = std::to_string(sqlite3_column_int(stmt, 4));
        }
    }
    sqlite3_finalize(stmt);
    return metadata;
}

std::vector<TimeRecord> SqliteReportRepository::get_time_records(const std::string& date) {
    std::vector<TimeRecord> records;
    sqlite3_stmt* stmt;
    
    // 注意：Repository 只负责取数据(ID)，路径解析由 Service/Querier 处理
    std::string sql = "SELECT start, end, project_id, duration, activity_remark FROM time_records WHERE date = ? ORDER BY logical_id ASC;";
    
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_STATIC);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            TimeRecord record;
            record.start_time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            record.end_time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            
            // 将 project_id 存入 project_path，供上层解析
            long long pid = sqlite3_column_int64(stmt, 2);
            record.project_path = std::to_string(pid); 
            
            record.duration_seconds = sqlite3_column_int64(stmt, 3);
            const unsigned char* rm = sqlite3_column_text(stmt, 4);
            if (rm) record.activityRemark = reinterpret_cast<const char*>(rm);
            
            records.push_back(record);
        }
    }
    sqlite3_finalize(stmt);
    return records;
}

std::vector<std::pair<long long, long long>> SqliteReportRepository::get_aggregated_project_stats(
    const std::string& start_date, const std::string& end_date) 
{
    std::vector<std::pair<long long, long long>> result;
    sqlite3_stmt* stmt;
    std::string sql = "SELECT project_id, SUM(duration) FROM time_records WHERE date >= ? AND date <= ? GROUP BY project_id;";
    
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, start_date.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, end_date.c_str(), -1, SQLITE_TRANSIENT);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            result.emplace_back(
                sqlite3_column_int64(stmt, 0),
                sqlite3_column_int64(stmt, 1)
            );
        }
    }
    sqlite3_finalize(stmt);
    return result;
}

std::map<std::string, long long> SqliteReportRepository::get_day_generated_stats(const std::string& date) {
    std::map<std::string, long long> stats;
    sqlite3_stmt* stmt;
    // 获取日报特有的统计列
    std::string sql = "SELECT "
                      "sleep_total_time, "
                      "total_exercise_time, anaerobic_time, cardio_time, "
                      "grooming_time, "
                      "study_time, "
                      "recreation_time, recreation_zhihu_time, recreation_bilibili_time, recreation_douyin_time "
                      "FROM days WHERE date = ?;";

    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; ++i) {
                const char* col_name = sqlite3_column_name(stmt, i);
                if (col_name) {
                    long long val = 0;
                    if (sqlite3_column_type(stmt, i) != SQLITE_NULL) {
                        val = sqlite3_column_int64(stmt, i);
                    }
                    stats[std::string(col_name)] = val;
                }
            }
        }
    }
    sqlite3_finalize(stmt);
    return stats;
}

int SqliteReportRepository::get_actual_active_days(const std::string& start_date, const std::string& end_date) {
    int actual_days = 0;
    sqlite3_stmt* stmt;
    std::string sql = "SELECT COUNT(DISTINCT date) FROM time_records WHERE date >= ? AND date <= ?;";
    
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, start_date.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, end_date.c_str(), -1, SQLITE_TRANSIENT);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            actual_days = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    return actual_days;
}

// --- Bulk Methods Implementation ---

std::map<std::string, DailyReportData> SqliteReportRepository::get_all_days_metadata() {
    std::map<std::string, DailyReportData> results;
    sqlite3_stmt* stmt;
    
    const char* sql = 
        "SELECT date, status, sleep, remark, getup_time, exercise, "
        "sleep_total_time, total_exercise_time, anaerobic_time, cardio_time, grooming_time, "
        "study_time, recreation_time, recreation_zhihu_time, recreation_bilibili_time, recreation_douyin_time "
        "FROM days ORDER BY date ASC;";

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        const std::vector<std::string> stat_cols = {
            "sleep_total_time", "total_exercise_time", "anaerobic_time", "cardio_time", "grooming_time",
            "study_time", "recreation_time", "recreation_zhihu_time", "recreation_bilibili_time", "recreation_douyin_time"
        };

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* date_cstr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            if (!date_cstr) continue;
            std::string date(date_cstr);
            
            DailyReportData& data = results[date];
            data.date = date;
            
            // Metadata
            data.metadata.status = std::to_string(sqlite3_column_int(stmt, 1));
            data.metadata.sleep = std::to_string(sqlite3_column_int(stmt, 2));
            const unsigned char* rem = sqlite3_column_text(stmt, 3);
            if (rem) data.metadata.remark = reinterpret_cast<const char*>(rem);
            const unsigned char* getup = sqlite3_column_text(stmt, 4);
            if (getup) data.metadata.getup_time = reinterpret_cast<const char*>(getup);
            data.metadata.exercise = std::to_string(sqlite3_column_int(stmt, 5));
            
            // Generated Stats
            for (size_t i = 0; i < stat_cols.size(); ++i) {
                // start index 6
                data.stats[stat_cols[i]] = sqlite3_column_int64(stmt, 6 + static_cast<int>(i));
            }
        }
    }
    sqlite3_finalize(stmt);
    return results;
}

std::vector<std::pair<std::string, TimeRecord>> SqliteReportRepository::get_all_time_records_with_date() {
    std::vector<std::pair<std::string, TimeRecord>> results;
    sqlite3_stmt* stmt;
    
    const char* sql = 
        "SELECT date, start, end, project_id, duration, activity_remark "
        "FROM time_records ORDER BY date ASC, logical_id ASC;";

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* date_cstr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            if (!date_cstr) continue;
            
            TimeRecord record;
            record.start_time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            record.end_time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            
            // 存 ID 到 project_path
            long long pid = sqlite3_column_int64(stmt, 3);
            record.project_path = std::to_string(pid); 
            
            record.duration_seconds = sqlite3_column_int64(stmt, 4);
            const unsigned char* ar = sqlite3_column_text(stmt, 5);
            if (ar) record.activityRemark = reinterpret_cast<const char*>(ar);

            results.emplace_back(std::string(date_cstr), record);
        }
    }
    sqlite3_finalize(stmt);
    return results;
}

std::map<std::string, std::vector<std::pair<long long, long long>>> SqliteReportRepository::get_all_months_project_stats() {
    std::map<std::string, std::vector<std::pair<long long, long long>>> results;
    sqlite3_stmt* stmt;
    
    const char* sql = 
        "SELECT strftime('%Y-%m', date) as ym, project_id, SUM(duration) "
        "FROM time_records GROUP BY ym, project_id ORDER BY ym ASC;";

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* ym_ptr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            if (!ym_ptr) continue;
            std::string ym(ym_ptr);
            long long pid = sqlite3_column_int64(stmt, 1);
            long long duration = sqlite3_column_int64(stmt, 2);
            
            results[ym].emplace_back(pid, duration);
        }
    }
    sqlite3_finalize(stmt);
    return results;
}

std::map<std::string, int> SqliteReportRepository::get_all_months_active_days() {
    std::map<std::string, int> results;
    sqlite3_stmt* stmt;
    
    const char* sql = 
        "SELECT strftime('%Y-%m', date) as ym, COUNT(DISTINCT date) "
        "FROM time_records GROUP BY ym;";

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* ym_ptr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            if (!ym_ptr) continue;
            results[std::string(ym_ptr)] = sqlite3_column_int(stmt, 1);
        }
    }
    sqlite3_finalize(stmt);
    return results;
}
