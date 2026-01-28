// serializer/core/log_deserializer.cpp
#include "serializer/core/log_deserializer.hpp"
#include "common/utils/string_utils.hpp"
#include <iostream>
#include <stdexcept>

namespace serializer::core {

// 辅助宏：安全的从对象获取值，如果 key 不存在或类型不匹配则返回默认值
static int64_t get_int_safe(yyjson_val *obj, const char *key, int64_t def = 0) {
  yyjson_val *val = yyjson_obj_get(obj, key);
  if (val && yyjson_is_int(val))
    return yyjson_get_int(val);
  if (val && yyjson_is_uint(val))
    return (int64_t)yyjson_get_uint(val); // 兼容 uint
  return def;
}

static std::string get_str_safe(yyjson_val *obj, const char *key,
                                const char *def = "") {
  yyjson_val *val = yyjson_obj_get(obj, key);
  if (val && yyjson_is_str(val))
    return std::string(yyjson_get_str(val));
  return std::string(def);
}

DailyLog LogDeserializer::deserialize(yyjson_val *day_json) {
  DailyLog day;
  if (!day_json || !yyjson_is_obj(day_json)) {
    throw std::runtime_error("Invalid JSON object for DailyLog");
  }

  yyjson_val *headers = yyjson_obj_get(day_json, "headers");
  yyjson_val *generated_stats = yyjson_obj_get(day_json, "generated_stats");

  if (!headers)
    throw std::runtime_error("Missing 'headers' field");
  if (!generated_stats)
    throw std::runtime_error("Missing 'generated_stats' field");

  // 1. Headers -> Basic Info
  yyjson_val *date_val = yyjson_obj_get(headers, "date");
  if (!date_val || !yyjson_is_str(date_val)) {
    throw std::runtime_error("Missing or invalid 'date' in headers");
  }
  day.date_ = yyjson_get_str(date_val);

  day.has_study_activity_ = (get_int_safe(headers, "status") != 0);
  day.has_sleep_activity_ = (get_int_safe(headers, "sleep") != 0);
  day.has_exercise_activity_ = (get_int_safe(headers, "exercise") != 0);

  std::string getup = get_str_safe(headers, "getup", "00:00");
  if (getup == "Null") {
    day.is_continuation_ = true;
    day.getup_time_ = "";
  } else {
    day.is_continuation_ = false;
    day.getup_time_ = getup;
  }

  std::string remark = get_str_safe(headers, "remark");
  if (!remark.empty()) {
    day.general_remarks_ = SplitString(remark, '\n');
  }

  day.activity_count_ = (int)get_int_safe(headers, "activity_count");

  // 2. Generated Stats -> Stats Struct
  day.stats_.sleep_night_time_ =
      get_int_safe(generated_stats, "sleep_night_time");
  day.stats_.sleep_day_time_ = get_int_safe(generated_stats, "sleep_day_time");
  day.stats_.sleep_total_time_ =
      get_int_safe(generated_stats, "sleep_total_time");

  day.stats_.total_exercise_time_ =
      get_int_safe(generated_stats, "total_exercise_time");
  day.stats_.cardio_time_ = get_int_safe(generated_stats, "cardio_time");
  day.stats_.anaerobic_time_ = get_int_safe(generated_stats, "anaerobic_time");

  day.stats_.grooming_time_ = get_int_safe(generated_stats, "grooming_time");
  day.stats_.toilet_time_ = get_int_safe(generated_stats, "toilet_time");
  day.stats_.gaming_time_ = get_int_safe(generated_stats, "gaming_time");

  day.stats_.recreation_time_ =
      get_int_safe(generated_stats, "recreation_time");
  day.stats_.recreation_zhihu_time_ =
      get_int_safe(generated_stats, "recreation_zhihu_time");
  day.stats_.recreation_bilibili_time_ =
      get_int_safe(generated_stats, "recreation_bilibili_time");
  day.stats_.recreation_douyin_time_ =
      get_int_safe(generated_stats, "recreation_douyin_time");

  day.stats_.study_time_ = get_int_safe(generated_stats, "total_study_time");

  // 3. Activities
  yyjson_val *activities_array = yyjson_obj_get(day_json, "activities");
  if (activities_array && yyjson_is_arr(activities_array)) {
    size_t idx, max;
    yyjson_val *activity_json;
    yyjson_arr_foreach(activities_array, idx, max, activity_json) {
      BaseActivityRecord record;
      record.logical_id_ = (int)get_int_safe(activity_json, "logical_id");
      record.start_timestamp_ = get_int_safe(activity_json, "start_timestamp");
      record.end_timestamp_ = get_int_safe(activity_json, "end_timestamp");
      record.start_time_str_ = get_str_safe(activity_json, "start_time");
      record.end_time_str_ = get_str_safe(activity_json, "end_time");
      record.duration_seconds_ =
          get_int_safe(activity_json, "duration_seconds");

      yyjson_val *remark_val = yyjson_obj_get(activity_json, "activity_remark");
      if (remark_val && yyjson_is_str(remark_val)) {
        record.remark_ = yyjson_get_str(remark_val);
      }

      yyjson_val *activity_details = yyjson_obj_get(activity_json, "activity");
      if (activity_details) {
        record.project_path_ = get_str_safe(activity_details, "project_path");
      }

      day.processed_activities_.push_back(record);
    }
  }

  return day;
}

} // namespace serializer::core
