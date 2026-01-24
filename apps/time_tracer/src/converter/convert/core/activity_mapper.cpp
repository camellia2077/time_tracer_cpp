#include "converter/convert/core/activity_mapper.hpp"
#include "common/utils/time_utils.hpp"
#include "common/utils/string_utils.hpp"
#include <algorithm>
#include <sstream>

// [Fix] 类型重命名
ActivityMapper::ActivityMapper(const LogMapperConfig& config)
    : config_(config) {}

void ActivityMapper::map_activities(DailyLog& day) {
    day.processedActivities.clear();
    if (day.getupTime.empty() && !day.isContinuation) return;
    
    std::string startTime = day.getupTime;

    for (const auto& rawEvent : day.rawEvents) {
        bool is_wake = std::find(config_.wake_keywords.begin(), config_.wake_keywords.end(), rawEvent.description) != config_.wake_keywords.end();
        
        if (is_wake) {
            if (startTime.empty()) {
                 startTime = TimeUtils::formatTime(rawEvent.endTimeStr);
            }
            continue;
        }

        std::string formattedEventEndTime = TimeUtils::formatTime(rawEvent.endTimeStr);
        std::string mappedDescription = rawEvent.description;
        
        auto mapIt = config_.text_mapping.find(mappedDescription);
        if (mapIt != config_.text_mapping.end()) {
            mappedDescription = mapIt->second;
        }

        auto durMapIt = config_.text_duration_mapping.find(mappedDescription);
        if (durMapIt != config_.text_duration_mapping.end()) {
            mappedDescription = durMapIt->second;
        }
        
        auto durationRulesIt = config_.duration_mappings.find(mappedDescription);
        if (durationRulesIt != config_.duration_mappings.end()) {
            int duration = TimeUtils::calculateDurationMinutes(startTime, formattedEventEndTime);
            for (const auto& rule : durationRulesIt->second) {
                if (duration < rule.less_than_minutes) {
                    mappedDescription = rule.value; 
                    break; 
                }
            }
        }
        
        if (!startTime.empty()) {
            std::vector<std::string> parts = split_string(mappedDescription, '_');
            if (!parts.empty()) {
                BaseActivityRecord activity;
                activity.start_time_str = startTime;
                activity.end_time_str = formattedEventEndTime;
                
                const auto& topParentsMap = config_.top_parent_mapping;
                auto map_it = topParentsMap.find(parts[0]);
                if (map_it != topParentsMap.end()) {
                    parts[0] = map_it->second;
                } else {
                    auto init_map_it = config_.initial_top_parents.find(parts[0]);
                    if (init_map_it != config_.initial_top_parents.end()) {
                        parts[0] = init_map_it->second;
                    }
                }
                
                std::stringstream ss;
                for (size_t i = 0; i < parts.size(); ++i) {
                    ss << parts[i] << (i < parts.size() - 1 ? "_" : "");
                }
                activity.project_path = ss.str();
                if (!rawEvent.remark.empty()) activity.remark = rawEvent.remark;
                
                day.processedActivities.push_back(activity);
            }
        }
        startTime = formattedEventEndTime;
    }
}