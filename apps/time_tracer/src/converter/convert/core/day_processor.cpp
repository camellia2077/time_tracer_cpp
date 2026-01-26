// converter/convert/core/day_processor.cpp
#include "converter/convert/core/day_processor.hpp"
#include "day_stats.hpp"
#include "activity_mapper.hpp"
#include "common/utils/time_utils.hpp"

// [Fix] 类型重命名
DayProcessor::DayProcessor(const LogMapperConfig& config) : config_(config) {}

void DayProcessor::process(DailyLog& previousDay, DailyLog& dayToProcess) {
    if (dayToProcess.date.empty()) return;

    // ActivityMapper 构造函数现在接受 LogMapperConfig
    ActivityMapper activity_mapper(config_);
    activity_mapper.map_activities(dayToProcess);

    if (!previousDay.date.empty() && !previousDay.rawEvents.empty() && !dayToProcess.getupTime.empty() && !dayToProcess.isContinuation) {
        std::string lastEventTime = TimeUtils::formatTime(previousDay.rawEvents.back().endTimeStr);

        BaseActivityRecord sleepActivity;
        sleepActivity.start_time_str = lastEventTime;
        sleepActivity.end_time_str = dayToProcess.getupTime;
        sleepActivity.project_path = "sleep_night"; 

        dayToProcess.processedActivities.insert(dayToProcess.processedActivities.begin(), sleepActivity);
        dayToProcess.hasSleepActivity = true;
    }

    if (dayToProcess.isContinuation && !previousDay.rawEvents.empty()) {
        dayToProcess.getupTime = TimeUtils::formatTime(previousDay.rawEvents.back().endTimeStr);
    }
    
    DayStats stats_calculator;
    stats_calculator.calculate_stats(dayToProcess);
}