// converter/convert/core/day_stats.cpp
#include "converter/convert/core/day_stats.hpp"
#include "converter/convert/core/stats_rules.hpp"
#include "common/utils/time_utils.hpp" // [DRY]
#include <algorithm>
#include <string>

void DayStats::calculate_stats(DailyLog& day) {
    day.activityCount = day.processedActivities.size();
    day.stats = {};
    day.hasStudyActivity = false;
    day.hasExerciseActivity = false;

    long long activity_sequence = 1;
    long long date_as_long = 0;
    try {
        std::string temp_date = day.date; 
        temp_date.erase(std::remove(temp_date.begin(), temp_date.end(), '-'), temp_date.end());
        date_as_long = std::stoll(temp_date); 
    } catch (...) {}

    for (auto& activity : day.processedActivities) {
        activity.logical_id = date_as_long * 10000 + activity_sequence++;
        
        // [DRY] 调用 TimeUtils
        activity.duration_seconds = TimeUtils::calculateDurationSeconds(activity.start_time_str, activity.end_time_str);
        
        activity.start_timestamp = TimeUtils::timeStringToTimestamp(day.date, activity.start_time_str, false, 0);
        activity.end_timestamp = TimeUtils::timeStringToTimestamp(day.date, activity.end_time_str, true, activity.start_timestamp);

        if (activity.project_path.rfind("study", 0) == 0) { 
            day.hasStudyActivity = true;
        }
        if (activity.project_path.rfind("exercise", 0) == 0) {
            day.hasExerciseActivity = true;
        }
        
        if (activity.project_path == "sleep_night") {
            day.stats.sleep_night_time += activity.duration_seconds;
        }
        if (activity.project_path == "sleep_day") {
            day.stats.sleep_day_time += activity.duration_seconds;
        }

        for (const auto& rule : StatsRules::rules) {
            if (activity.project_path.find(rule.match_path) == 0) {
                (day.stats.*(rule.member)) += activity.duration_seconds;
            }
        }
    }

    day.stats.sleep_total_time = day.stats.sleep_night_time + day.stats.sleep_day_time;
}