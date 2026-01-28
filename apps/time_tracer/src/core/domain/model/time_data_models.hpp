// core/domain/model/time_data_models.hpp
#ifndef CORE_DOMAIN_MODEL_TIME_DATA_MODELS_HPP_
#define CORE_DOMAIN_MODEL_TIME_DATA_MODELS_HPP_

#include <optional>
#include <string>

// 统一的统计结构 (对应原 GeneratedStats，但字段名为 snake_case)
struct ActivityStats {
  int sleep_night_time_ = 0;
  int sleep_day_time_ = 0;
  int sleep_total_time_ = 0;

  int total_exercise_time_ = 0;
  int cardio_time_ = 0;
  int anaerobic_time_ = 0;

  int grooming_time_ = 0;
  int toilet_time_ = 0;
  int gaming_time_ = 0;

  int recreation_time_ = 0;
  int recreation_zhihu_time_ = 0;
  int recreation_bilibili_time_ = 0;
  int recreation_douyin_time_ = 0;

  int study_time_ = 0;
};

// 统一的基础活动记录 (对应原 Activity / TimeRecordInternal)
struct BaseActivityRecord {
  long long logical_id_ = 0;
  long long start_timestamp_ = 0;
  long long end_timestamp_ = 0;

  std::string start_time_str_; // 原 startTime / start
  std::string end_time_str_;   // 原 endTime / end
  std::string project_path_;

  int duration_seconds_ = 0;          // 原 durationSeconds
  std::optional<std::string> remark_; // 原 activityRemark
};

#endif // CORE_DOMAIN_MODEL_TIME_DATA_MODELS_HPP_
