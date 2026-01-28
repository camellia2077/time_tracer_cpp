// converter/convert/core/log_linker.cpp
#include "converter/convert/core/log_linker.hpp"
#include "common/ansi_colors.hpp"
#include "common/utils/time_utils.hpp"
#include "converter/convert/core/day_stats.hpp"
#include <iostream>

// [Fix] 类型重命名
LogLinker::LogLinker(const LogLinkerConfig &config) : config_(config) {}

void LogLinker::LinkLogs(
    std::map<std::string, std::vector<DailyLog>> &data_map) {
  DailyLog *prev_month_last_day = nullptr;
  int linked_count = 0;

  for (auto &[month_key, days] : data_map) {
    if (days.empty())
      continue;
    DailyLog &current_first_day = days.front();

    if (prev_month_last_day != nullptr) {
      bool has_valid_getup = !current_first_day.getup_time_.empty() &&
                             current_first_day.getup_time_ != "00:00";
      bool missing_sleep = !current_first_day.has_sleep_activity_;

      if (has_valid_getup && missing_sleep) {
        ProcessCrossDay(current_first_day, *prev_month_last_day);
        linked_count++;
      }
    }
    prev_month_last_day = &days.back();
  }

  if (linked_count > 0) {
    std::cout << kGreenColor << "  [LogLinker] Linked " << linked_count
              << " cross-month sleep records." << kResetColor << std::endl;
  }
}

void LogLinker::ProcessCrossDay(DailyLog &current_day,
                                const DailyLog &prev_day) {
  if (prev_day.raw_events_.empty())
    return;

  std::string last_end_time_raw = prev_day.raw_events_.back().end_time_str_;
  std::string sleep_start_time = TimeUtils::FormatTime(last_end_time_raw);
  std::string sleep_end_time = current_day.getup_time_;

  BaseActivityRecord sleepActivity;
  sleepActivity.start_time_str_ = sleep_start_time;
  sleepActivity.end_time_str_ = sleep_end_time;

  sleepActivity.project_path_ = config_.generated_sleep_project_path_;

  current_day.processed_activities_.insert(
      current_day.processed_activities_.begin(), sleepActivity);
  current_day.has_sleep_activity_ = true;

  RecalculateStats(current_day);
}

void LogLinker::RecalculateStats(DailyLog &day) {
  DayStats stats_calculator;
  stats_calculator.CalculateStats(day);
}
