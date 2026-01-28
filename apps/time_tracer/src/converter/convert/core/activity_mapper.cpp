// converter/convert/core/activity_mapper.cpp
#include "converter/convert/core/activity_mapper.hpp"
#include "common/utils/string_utils.hpp"
#include "common/utils/time_utils.hpp"
#include <algorithm>
#include <sstream>

// [Fix] 类型重命名
ActivityMapper::ActivityMapper(const LogMapperConfig &config)
    : config_(config) {}

void ActivityMapper::MapActivities(DailyLog &day) {
  day.processed_activities_.clear();
  if (day.getup_time_.empty() && !day.is_continuation_)
    return;

  std::string startTime = day.getup_time_;

  for (const auto &rawEvent : day.raw_events_) {
    bool is_wake =
        std::find(config_.wake_keywords_.begin(), config_.wake_keywords_.end(),
                  rawEvent.description_) != config_.wake_keywords_.end();

    if (is_wake) {
      if (startTime.empty()) {
        startTime = TimeUtils::FormatTime(rawEvent.end_time_str_);
      }
      continue;
    }

    std::string formattedEventEndTime =
        TimeUtils::FormatTime(rawEvent.end_time_str_);
    std::string mappedDescription = rawEvent.description_;

    auto mapIt = config_.text_mapping_.find(mappedDescription);
    if (mapIt != config_.text_mapping_.end()) {
      mappedDescription = mapIt->second;
    }

    auto durMapIt = config_.text_duration_mapping_.find(mappedDescription);
    if (durMapIt != config_.text_duration_mapping_.end()) {
      mappedDescription = durMapIt->second;
    }

    auto durationRulesIt = config_.duration_mappings_.find(mappedDescription);
    if (durationRulesIt != config_.duration_mappings_.end()) {
      int duration =
          TimeUtils::CalculateDurationMinutes(startTime, formattedEventEndTime);
      for (const auto &rule : durationRulesIt->second) {
        if (duration < rule.less_than_minutes_) {
          mappedDescription = rule.value_;
          break;
        }
      }
    }

    if (!startTime.empty()) {
      std::vector<std::string> parts = SplitString(mappedDescription, '_');
      if (!parts.empty()) {
        BaseActivityRecord activity;
        activity.start_time_str_ = startTime;
        activity.end_time_str_ = formattedEventEndTime;

        const auto &topParentsMap = config_.top_parent_mapping_;
        auto map_it = topParentsMap.find(parts[0]);
        if (map_it != topParentsMap.end()) {
          parts[0] = map_it->second;
        } else {
          auto init_map_it = config_.initial_top_parents_.find(parts[0]);
          if (init_map_it != config_.initial_top_parents_.end()) {
            parts[0] = init_map_it->second;
          }
        }

        std::stringstream ss;
        for (size_t i = 0; i < parts.size(); ++i) {
          ss << parts[i] << (i < parts.size() - 1 ? "_" : "");
        }
        activity.project_path_ = ss.str();
        if (!rawEvent.remark_.empty())
          activity.remark_ = rawEvent.remark_;

        day.processed_activities_.push_back(activity);
      }
    }
    startTime = formattedEventEndTime;
  }
}
