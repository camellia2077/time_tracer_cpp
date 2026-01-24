#include "converter/convert/core/log_linker.hpp"
#include "converter/convert/core/day_stats.hpp"
#include "common/utils/time_utils.hpp"
#include "common/ansi_colors.hpp"
#include <iostream>

// [Fix] 类型重命名
LogLinker::LogLinker(const LogLinkerConfig& config) : config_(config) {}

void LogLinker::link_logs(std::map<std::string, std::vector<DailyLog>>& data_map) {
    DailyLog* prev_month_last_day = nullptr;
    int linked_count = 0;

    for (auto& [month_key, days] : data_map) {
        if (days.empty()) continue;
        DailyLog& current_first_day = days.front();
        
        if (prev_month_last_day != nullptr) {
            bool has_valid_getup = !current_first_day.getupTime.empty() && current_first_day.getupTime != "00:00";
            bool missing_sleep = !current_first_day.hasSleepActivity;

            if (has_valid_getup && missing_sleep) {
                process_cross_day(current_first_day, *prev_month_last_day);
                linked_count++;
            }
        }
        prev_month_last_day = &days.back();
    }

    if (linked_count > 0) {
        std::cout << GREEN_COLOR << "  [LogLinker] Linked " << linked_count << " cross-month sleep records." << RESET_COLOR << std::endl;
    }
}

void LogLinker::process_cross_day(DailyLog& current_day, const DailyLog& prev_day) {
    if (prev_day.rawEvents.empty()) return;

    std::string last_end_time_raw = prev_day.rawEvents.back().endTimeStr;
    std::string sleep_start_time = TimeUtils::formatTime(last_end_time_raw);
    std::string sleep_end_time = current_day.getupTime;

    BaseActivityRecord sleepActivity;
    sleepActivity.start_time_str = sleep_start_time;
    sleepActivity.end_time_str = sleep_end_time;
    
    sleepActivity.project_path = config_.generated_sleep_project_path;

    current_day.processedActivities.insert(current_day.processedActivities.begin(), sleepActivity);
    current_day.hasSleepActivity = true;
    
    recalculate_stats(current_day);
}

void LogLinker::recalculate_stats(DailyLog& day) {
    DayStats stats_calculator;
    stats_calculator.calculate_stats(day);
}