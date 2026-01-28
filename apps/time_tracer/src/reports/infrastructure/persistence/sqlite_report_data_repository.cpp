// reports/infrastructure/persistence/sqlite_report_data_repository.cpp
#include "reports/infrastructure/persistence/sqlite_report_data_repository.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

#include "reports/data/cache/project_name_cache.hpp"

SqliteReportDataRepository::SqliteReportDataRepository(sqlite3 *db) : db_(db) {
  if (!db_)
    throw std::invalid_argument("Database connection cannot be null.");

  // 核心修复：确保项目名称缓存已加载
  ProjectNameCache::instance().ensure_loaded(db_);
}

// --- Single Query Methods ---

DayMetadata
SqliteReportDataRepository::get_day_metadata(const std::string &date) {
  DayMetadata metadata;
  sqlite3_stmt *stmt;
  std::string sql = "SELECT status, sleep, remark, getup_time, exercise FROM "
                    "days WHERE date = ?;";

  if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
      metadata.status_ = std::to_string(sqlite3_column_int(stmt, 0));
      metadata.sleep_ = std::to_string(sqlite3_column_int(stmt, 1));
      const unsigned char *r = sqlite3_column_text(stmt, 2);
      if (r)
        metadata.remark_ = reinterpret_cast<const char *>(r);
      const unsigned char *g = sqlite3_column_text(stmt, 3);
      if (g)
        metadata.getup_time_ = reinterpret_cast<const char *>(g);
      metadata.exercise_ = std::to_string(sqlite3_column_int(stmt, 4));
    }
  }
  sqlite3_finalize(stmt);
  return metadata;
}

std::vector<TimeRecord>
SqliteReportDataRepository::get_time_records(const std::string &date) {
  std::vector<TimeRecord> records;
  sqlite3_stmt *stmt;

  std::string sql = "SELECT start, end, project_id, duration, activity_remark "
                    "FROM time_records WHERE date = ? ORDER BY logical_id ASC;";

  if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      TimeRecord record;
      record.start_time_ =
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
      record.end_time_ =
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));

      long long pid = sqlite3_column_int64(stmt, 2);
      record.project_path_ = std::to_string(pid);

      record.duration_seconds_ = sqlite3_column_int64(stmt, 3);
      const unsigned char *rm = sqlite3_column_text(stmt, 4);
      if (rm)
        record.activity_remark_ = reinterpret_cast<const char *>(rm);

      records.push_back(record);
    }
  }
  sqlite3_finalize(stmt);
  return records;
}

std::vector<std::pair<long long, long long>>
SqliteReportDataRepository::get_aggregated_project_stats(
    const std::string &start_date, const std::string &end_date) {
  std::vector<std::pair<long long, long long>> result;
  sqlite3_stmt *stmt;
  std::string sql = "SELECT project_id, SUM(duration) FROM time_records WHERE "
                    "date >= ? AND date <= ? GROUP BY project_id;";

  if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    sqlite3_bind_text(stmt, 1, start_date.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, end_date.c_str(), -1, SQLITE_TRANSIENT);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      result.emplace_back(sqlite3_column_int64(stmt, 0),
                          sqlite3_column_int64(stmt, 1));
    }
  }
  sqlite3_finalize(stmt);
  return result;
}

std::map<std::string, long long>
SqliteReportDataRepository::get_day_generated_stats(const std::string &date) {
  std::map<std::string, long long> stats;
  sqlite3_stmt *stmt;
  std::string sql = "SELECT "
                    "sleep_total_time, "
                    "total_exercise_time, anaerobic_time, cardio_time, "
                    "grooming_time, "
                    "study_time, "
                    "recreation_time, recreation_zhihu_time, "
                    "recreation_bilibili_time, recreation_douyin_time "
                    "FROM days WHERE date = ?;";

  if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
      int col_count = sqlite3_column_count(stmt);
      for (int i = 0; i < col_count; ++i) {
        const char *col_name = sqlite3_column_name(stmt, i);
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

int SqliteReportDataRepository::get_actual_active_days(
    const std::string &start_date, const std::string &end_date) {
  int actual_days = 0;
  sqlite3_stmt *stmt;
  std::string sql = "SELECT COUNT(DISTINCT date) FROM time_records WHERE date "
                    ">= ? AND date <= ?;";

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

std::map<std::string, DailyReportData>
SqliteReportDataRepository::get_all_days_metadata() {
  std::map<std::string, DailyReportData> results;
  sqlite3_stmt *stmt;

  const char *sql = "SELECT date, status, sleep, remark, getup_time, exercise, "
                    "sleep_total_time, total_exercise_time, anaerobic_time, "
                    "cardio_time, grooming_time, "
                    "study_time, recreation_time, recreation_zhihu_time, "
                    "recreation_bilibili_time, recreation_douyin_time "
                    "FROM days ORDER BY date ASC;";

  if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) == SQLITE_OK) {
    const std::vector<std::string> stat_cols = {"sleep_total_time",
                                                "total_exercise_time",
                                                "anaerobic_time",
                                                "cardio_time",
                                                "grooming_time",
                                                "study_time",
                                                "recreation_time",
                                                "recreation_zhihu_time",
                                                "recreation_bilibili_time",
                                                "recreation_douyin_time"};

    while (sqlite3_step(stmt) == SQLITE_ROW) {
      const char *date_cstr =
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
      if (!date_cstr)
        continue;
      std::string date(date_cstr);

      DailyReportData &data = results[date];
      data.date_ = date;

      // Metadata
      data.metadata_.status_ = std::to_string(sqlite3_column_int(stmt, 1));
      data.metadata_.sleep_ = std::to_string(sqlite3_column_int(stmt, 2));
      const unsigned char *rem = sqlite3_column_text(stmt, 3);
      if (rem)
        data.metadata_.remark_ = reinterpret_cast<const char *>(rem);
      const unsigned char *getup = sqlite3_column_text(stmt, 4);
      if (getup)
        data.metadata_.getup_time_ = reinterpret_cast<const char *>(getup);
      data.metadata_.exercise_ = std::to_string(sqlite3_column_int(stmt, 5));

      // Generated Stats
      for (size_t i = 0; i < stat_cols.size(); ++i) {
        // start index 6
        data.stats_[stat_cols[i]] =
            sqlite3_column_int64(stmt, 6 + static_cast<int>(i));
      }
    }
  }
  sqlite3_finalize(stmt);
  return results;
}

std::vector<std::pair<std::string, TimeRecord>>
SqliteReportDataRepository::get_all_time_records_with_date() {
  std::vector<std::pair<std::string, TimeRecord>> results;
  sqlite3_stmt *stmt;

  const char *sql =
      "SELECT date, start, end, project_id, duration, activity_remark "
      "FROM time_records ORDER BY date ASC, logical_id ASC;";

  if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      const char *date_cstr =
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
      if (!date_cstr)
        continue;

      TimeRecord record;
      record.start_time_ =
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
      record.end_time_ =
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));

      long long pid = sqlite3_column_int64(stmt, 3);
      record.project_path_ = std::to_string(pid);

      record.duration_seconds_ = sqlite3_column_int64(stmt, 4);
      const unsigned char *ar =
          reinterpret_cast<const unsigned char *>(sqlite3_column_text(stmt, 5));
      if (ar)
        record.activity_remark_ = reinterpret_cast<const char *>(ar);

      results.emplace_back(std::string(date_cstr), record);
    }
  }
  sqlite3_finalize(stmt);
  return results;
}

std::map<std::string, std::vector<std::pair<long long, long long>>>
SqliteReportDataRepository::get_all_months_project_stats() {
  std::map<std::string, std::vector<std::pair<long long, long long>>> results;
  sqlite3_stmt *stmt;

  const char *sql =
      "SELECT strftime('%Y-%m', date) as ym, project_id, SUM(duration) "
      "FROM time_records GROUP BY ym, project_id ORDER BY ym ASC;";

  if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      const char *ym_ptr =
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
      if (!ym_ptr)
        continue;
      std::string ym(ym_ptr);
      long long pid = sqlite3_column_int64(stmt, 1);
      long long duration = sqlite3_column_int64(stmt, 2);

      results[ym].emplace_back(pid, duration);
    }
  }
  sqlite3_finalize(stmt);
  return results;
}

std::map<std::string, int>
SqliteReportDataRepository::get_all_months_active_days() {
  std::map<std::string, int> results;
  sqlite3_stmt *stmt;

  const char *sql =
      "SELECT strftime('%Y-%m', date) as ym, COUNT(DISTINCT date) "
      "FROM time_records GROUP BY ym;";

  if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      const char *ym_ptr =
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
      if (!ym_ptr)
        continue;
      results[std::string(ym_ptr)] = sqlite3_column_int(stmt, 1);
    }
  }
  sqlite3_finalize(stmt);
  return results;
}

// [新增] Weekly Bulk Implementation

std::map<std::string, std::vector<std::pair<long long, long long>>>
SqliteReportDataRepository::get_all_weeks_project_stats() {
  std::map<std::string, std::vector<std::pair<long long, long long>>> results;
  sqlite3_stmt *stmt;

  // [修复] 使用 ISO 8601 逻辑的 SQL
  // 逻辑说明：
  // 1. date(date, 'weekday 0', '-3 days') 找到该周的“周四”
  // 2. strftime('%Y', ...) 获取周四所在的年份 (ISO Year)
  // 3. (strftime('%j', ...) - 1) / 7 + 1 计算 ISO Week Number
  const char *sql =
      "SELECT "
      "  strftime('%Y', date(date, '-3 days', 'weekday 4')) || '-W' || "
      "  printf('%02d', (strftime('%j', date(date, '-3 days', 'weekday 4')) - "
      "1) / 7 + 1) as iso_week, "
      "  project_id, "
      "  SUM(duration) "
      "FROM time_records "
      "GROUP BY iso_week, project_id "
      "ORDER BY iso_week ASC;";

  if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      const char *yw_ptr =
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
      if (!yw_ptr)
        continue;
      std::string yw(yw_ptr);
      long long pid = sqlite3_column_int64(stmt, 1);
      long long duration = sqlite3_column_int64(stmt, 2);

      results[yw].emplace_back(pid, duration);
    }
  }
  sqlite3_finalize(stmt);
  return results;
}

std::map<std::string, int>
SqliteReportDataRepository::get_all_weeks_active_days() {
  std::map<std::string, int> results;
  sqlite3_stmt *stmt;

  // [修复] 同样的 ISO 8601 逻辑
  const char *sql =
      "SELECT "
      "  strftime('%Y', date(date, '-3 days', 'weekday 4')) || '-W' || "
      "  printf('%02d', (strftime('%j', date(date, '-3 days', 'weekday 4')) - "
      "1) / 7 + 1) as iso_week, "
      "  COUNT(DISTINCT date) "
      "FROM time_records "
      "GROUP BY iso_week;";

  if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      const char *yw_ptr =
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
      if (!yw_ptr)
        continue;
      results[std::string(yw_ptr)] = sqlite3_column_int(stmt, 1);
    }
  }
  sqlite3_finalize(stmt);
  return results;
}

// [新增] Yearly Bulk Implementation

std::map<std::string, std::vector<std::pair<long long, long long>>>
SqliteReportDataRepository::get_all_years_project_stats() {
  std::map<std::string, std::vector<std::pair<long long, long long>>> results;
  sqlite3_stmt *stmt;

  const char *sql =
      "SELECT strftime('%Y', date) as y, project_id, SUM(duration) "
      "FROM time_records GROUP BY y, project_id ORDER BY y ASC;";

  if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      const char *y_ptr =
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
      if (!y_ptr)
        continue;
      std::string y_str(y_ptr);
      long long pid = sqlite3_column_int64(stmt, 1);
      long long duration = sqlite3_column_int64(stmt, 2);

      results[y_str].emplace_back(pid, duration);
    }
  }
  sqlite3_finalize(stmt);
  return results;
}

std::map<std::string, int>
SqliteReportDataRepository::get_all_years_active_days() {
  std::map<std::string, int> results;
  sqlite3_stmt *stmt;

  const char *sql = "SELECT strftime('%Y', date) as y, COUNT(DISTINCT date) "
                    "FROM time_records GROUP BY y;";

  if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      const char *y_ptr =
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
      if (!y_ptr)
        continue;
      results[std::string(y_ptr)] = sqlite3_column_int(stmt, 1);
    }
  }
  sqlite3_finalize(stmt);
  return results;
}
