// converter/convert/core/day_stats.cpp
#include "converter/convert/core/day_stats.hpp"
#include "common/utils/time_utils.hpp" // [DRY]
#include "converter/convert/core/stats_rules.hpp"
#include <algorithm>
#include <string>

void DayStats::CalculateStats(DailyLog &day) {
  day.activity_count_ = day.processed_activities_.size();
  day.stats_ = {};
  day.has_study_activity_ = false;
  day.has_exercise_activity_ = false;

  long long activity_sequence = 1;
  long long date_as_long = 0;
  try {
    std::string temp_date = day.date_;
    temp_date.erase(std::remove(temp_date.begin(), temp_date.end(), '-'),
                    temp_date.end());
    date_as_long = std::stoll(temp_date);
  } catch (...) {
  }

  for (auto &activity : day.processed_activities_) {
    activity.logical_id_ = date_as_long * 10000 + activity_sequence++;

    // [DRY] 调用 TimeUtils
    activity.duration_seconds_ = TimeUtils::CalculateDurationSeconds(
        activity.start_time_str_, activity.end_time_str_);

    activity.start_timestamp_ = TimeUtils::TimeStringToTimestamp(
        day.date_, activity.start_time_str_, false, 0);
    activity.end_timestamp_ = TimeUtils::TimeStringToTimestamp(
        day.date_, activity.end_time_str_, true, activity.start_timestamp_);

    if (activity.project_path_.rfind("study", 0) == 0) {
      day.has_study_activity_ = true;
    }
    if (activity.project_path_.rfind("exercise", 0) == 0) {
      day.has_exercise_activity_ = true;
    }

    if (activity.project_path_ == "sleep_night") {
      day.stats_.sleep_night_time_ += activity.duration_seconds_;
    }
    if (activity.project_path_ == "sleep_day") {
      day.stats_.sleep_day_time_ += activity.duration_seconds_;
    }

    for (const auto &rule : StatsRules::rules) {
      if (activity.project_path_.find(rule.match_path) == 0) {
        (day.stats_.*(rule.member)) += activity.duration_seconds_;
      }
    }
  }

  day.stats_.sleep_total_time_ =
      day.stats_.sleep_night_time_ + day.stats_.sleep_day_time_;
}
