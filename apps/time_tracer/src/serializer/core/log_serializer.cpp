// serializer/core/log_serializer.cpp
#include "serializer/core/log_serializer.hpp"

namespace serializer::core {

yyjson_mut_val *LogSerializer::serialize(yyjson_mut_doc *doc,
                                         const DailyLog &day) {
  if (day.date_.empty())
    return yyjson_mut_null(doc);

  yyjson_mut_val *day_obj = yyjson_mut_obj(doc);

  // 1. Headers
  yyjson_mut_val *headers_obj = yyjson_mut_obj(doc);
  yyjson_mut_obj_add_str(doc, headers_obj, "date", day.date_.c_str());
  yyjson_mut_obj_add_int(doc, headers_obj, "status",
                         day.has_study_activity_ ? 1 : 0);
  yyjson_mut_obj_add_int(doc, headers_obj, "exercise",
                         day.has_exercise_activity_ ? 1 : 0);
  yyjson_mut_obj_add_int(doc, headers_obj, "sleep",
                         day.has_sleep_activity_ ? 1 : 0);

  const char *getup_val =
      day.is_continuation_
          ? "Null"
          : (day.getup_time_.empty() ? "00:00" : day.getup_time_.c_str());
  yyjson_mut_obj_add_str(doc, headers_obj, "getup", getup_val);

  yyjson_mut_obj_add_int(doc, headers_obj, "activity_count",
                         day.activity_count_);

  if (!day.general_remarks_.empty()) {
    std::string full_remark;
    for (size_t i = 0; i < day.general_remarks_.size(); ++i) {
      full_remark += day.general_remarks_[i];
      if (i < day.general_remarks_.size() - 1) {
        full_remark += "\n";
      }
    }
    yyjson_mut_obj_add_str(doc, headers_obj, "remark", full_remark.c_str());
  } else {
    yyjson_mut_obj_add_str(doc, headers_obj, "remark", "");
  }
  yyjson_mut_obj_add_val(doc, day_obj, "headers", headers_obj);

  // 2. Activities
  yyjson_mut_val *activities = yyjson_mut_arr(doc);
  for (const auto &activity_data : day.processed_activities_) {
    yyjson_mut_val *activity_obj = yyjson_mut_obj(doc);
    yyjson_mut_obj_add_int(doc, activity_obj, "logical_id",
                           activity_data.logical_id_);
    yyjson_mut_obj_add_int(doc, activity_obj, "start_timestamp",
                           activity_data.start_timestamp_);
    yyjson_mut_obj_add_int(doc, activity_obj, "end_timestamp",
                           activity_data.end_timestamp_);
    yyjson_mut_obj_add_str(doc, activity_obj, "start_time",
                           activity_data.start_time_str_.c_str());
    yyjson_mut_obj_add_str(doc, activity_obj, "end_time",
                           activity_data.end_time_str_.c_str());
    yyjson_mut_obj_add_int(doc, activity_obj, "duration_seconds",
                           activity_data.duration_seconds_);

    if (activity_data.remark_.has_value()) {
      yyjson_mut_obj_add_str(doc, activity_obj, "activity_remark",
                             activity_data.remark_.value().c_str());
    } else {
      yyjson_mut_obj_add_null(doc, activity_obj, "activity_remark");
    }

    yyjson_mut_val *activity_details = yyjson_mut_obj(doc);
    yyjson_mut_obj_add_str(doc, activity_details, "project_path",
                           activity_data.project_path_.c_str());
    yyjson_mut_obj_add_val(doc, activity_obj, "activity", activity_details);

    yyjson_mut_arr_add_val(activities, activity_obj);
  }
  yyjson_mut_obj_add_val(doc, day_obj, "activities", activities);

  // 3. Generated Stats
  yyjson_mut_val *stats_obj = yyjson_mut_obj(doc);
  yyjson_mut_obj_add_int(doc, stats_obj, "sleep_night_time",
                         day.stats_.sleep_night_time_);
  yyjson_mut_obj_add_int(doc, stats_obj, "sleep_day_time",
                         day.stats_.sleep_day_time_);
  yyjson_mut_obj_add_int(doc, stats_obj, "sleep_total_time",
                         day.stats_.sleep_total_time_);
  yyjson_mut_obj_add_int(doc, stats_obj, "total_exercise_time",
                         day.stats_.total_exercise_time_);
  yyjson_mut_obj_add_int(doc, stats_obj, "cardio_time",
                         day.stats_.cardio_time_);
  yyjson_mut_obj_add_int(doc, stats_obj, "anaerobic_time",
                         day.stats_.anaerobic_time_);
  yyjson_mut_obj_add_int(doc, stats_obj, "grooming_time",
                         day.stats_.grooming_time_);
  yyjson_mut_obj_add_int(doc, stats_obj, "toilet_time",
                         day.stats_.toilet_time_);
  yyjson_mut_obj_add_int(doc, stats_obj, "gaming_time",
                         day.stats_.gaming_time_);
  yyjson_mut_obj_add_int(doc, stats_obj, "recreation_time",
                         day.stats_.recreation_time_);
  yyjson_mut_obj_add_int(doc, stats_obj, "recreation_zhihu_time",
                         day.stats_.recreation_zhihu_time_);
  yyjson_mut_obj_add_int(doc, stats_obj, "recreation_bilibili_time",
                         day.stats_.recreation_bilibili_time_);
  yyjson_mut_obj_add_int(doc, stats_obj, "recreation_douyin_time",
                         day.stats_.recreation_douyin_time_);
  yyjson_mut_obj_add_int(doc, stats_obj, "total_study_time",
                         day.stats_.study_time_);

  yyjson_mut_obj_add_val(doc, day_obj, "generated_stats", stats_obj);

  return day_obj;
}

} // namespace serializer::core
