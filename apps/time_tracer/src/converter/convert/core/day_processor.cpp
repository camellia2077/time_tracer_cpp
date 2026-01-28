// converter/convert/core/day_processor.cpp
#include "converter/convert/core/day_processor.hpp"
#include "activity_mapper.hpp"
#include "common/utils/time_utils.hpp"
#include "day_stats.hpp"

// [Fix] 类型重命名
DayProcessor::DayProcessor(const LogMapperConfig &config) : config_(config) {}

void DayProcessor::Process(DailyLog &previousDay, DailyLog &dayToProcess) {
  if (dayToProcess.date_.empty())
    return;

  // ActivityMapper 构造函数现在接受 LogMapperConfig
  ActivityMapper activity_mapper(config_);
  activity_mapper.MapActivities(dayToProcess);

  if (!previousDay.date_.empty() && !previousDay.raw_events_.empty() &&
      !dayToProcess.getup_time_.empty() && !dayToProcess.is_continuation_) {
    std::string lastEventTime =
        TimeUtils::FormatTime(previousDay.raw_events_.back().end_time_str_);

    if (lastEventTime != dayToProcess.getup_time_) {
      // 只有在两者不同时才进行跨天关联
      // ProcessCrossDay ...
      BaseActivityRecord sleepActivity;
      sleepActivity.start_time_str_ = lastEventTime;
      sleepActivity.end_time_str_ = dayToProcess.getup_time_;
      sleepActivity.project_path_ = "sleep_night";

      dayToProcess.processed_activities_.insert(
          dayToProcess.processed_activities_.begin(), sleepActivity);
      dayToProcess.has_sleep_activity_ = true;
    }
  }

  if (dayToProcess.is_continuation_ && !previousDay.raw_events_.empty()) {
    dayToProcess.getup_time_ =
        TimeUtils::FormatTime(previousDay.raw_events_.back().end_time_str_);
  }

  DayStats stats_calculator;
  stats_calculator.CalculateStats(dayToProcess);
}
