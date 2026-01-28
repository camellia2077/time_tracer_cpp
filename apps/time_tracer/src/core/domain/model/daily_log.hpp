// core/domain/model/daily_log.hpp
#ifndef CORE_DOMAIN_MODEL_DAILY_LOG_HPP_
#define CORE_DOMAIN_MODEL_DAILY_LOG_HPP_

#include "time_data_models.hpp"
#include <optional>
#include <string>
#include <vector>

// RawEvent 属于 Converter 解析阶段的中间产物，保留在此
struct RawEvent {
  std::string end_time_str_;
  std::string description_;
  std::string remark_;
};

// [核心修改] 移除 Activity 和 GeneratedStats 的定义

struct DailyLog {
  std::string date_;
  bool has_study_activity_ = false;
  bool has_exercise_activity_ = false;
  bool has_sleep_activity_ = false;

  std::string getup_time_;
  std::vector<std::string> general_remarks_;
  std::vector<RawEvent> raw_events_;

  // [核心修改] 使用 BaseActivityRecord
  std::vector<BaseActivityRecord> processed_activities_;

  bool is_continuation_ = false;

  int activity_count_ = 0;

  // [核心修改] 使用 ActivityStats，并重命名为 stats
  ActivityStats stats_;

  void Clear() {
    date_.clear();
    has_study_activity_ = false;
    has_exercise_activity_ = false;
    has_sleep_activity_ = false;
    getup_time_.clear();
    general_remarks_.clear();
    raw_events_.clear();
    processed_activities_.clear();
    is_continuation_ = false;
    activity_count_ = 0;
    stats_ = {}; // 重置为默认值
  }
};

#endif // CORE_DOMAIN_MODEL_DAILY_LOG_HPP_
