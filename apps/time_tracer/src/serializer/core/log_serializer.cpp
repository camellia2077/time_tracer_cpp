// serializer/core/log_serializer.cpp
#include "log_serializer.hpp"

namespace serializer::core {

yyjson_mut_val* LogSerializer::serialize(yyjson_mut_doc* doc, const DailyLog& day) {
    if (day.date.empty()) return yyjson_mut_null(doc);

    yyjson_mut_val* day_obj = yyjson_mut_obj(doc);
    
    // 1. Headers
    yyjson_mut_val* headers_obj = yyjson_mut_obj(doc);
    yyjson_mut_obj_add_str(doc, headers_obj, "date", day.date.c_str());
    yyjson_mut_obj_add_int(doc, headers_obj, "status", day.hasStudyActivity ? 1 : 0);
    yyjson_mut_obj_add_int(doc, headers_obj, "exercise", day.hasExerciseActivity ? 1 : 0);
    yyjson_mut_obj_add_int(doc, headers_obj, "sleep", day.hasSleepActivity ? 1 : 0);
    
    const char* getup_val = day.isContinuation ? "Null" : (day.getupTime.empty() ? "00:00" : day.getupTime.c_str());
    yyjson_mut_obj_add_str(doc, headers_obj, "getup", getup_val);
    
    yyjson_mut_obj_add_int(doc, headers_obj, "activity_count", day.activityCount);

    if (!day.generalRemarks.empty()) {
        std::string full_remark;
        for (size_t i = 0; i < day.generalRemarks.size(); ++i) {
            full_remark += day.generalRemarks[i];
            if (i < day.generalRemarks.size() - 1) {
                full_remark += "\n"; 
            }
        }
        yyjson_mut_obj_add_str(doc, headers_obj, "remark", full_remark.c_str());
    } else {
        yyjson_mut_obj_add_str(doc, headers_obj, "remark", "");
    }
    yyjson_mut_obj_add_val(doc, day_obj, "headers", headers_obj);
    
    // 2. Activities
    yyjson_mut_val* activities = yyjson_mut_arr(doc);
    for (const auto& activity_data : day.processedActivities) {
        yyjson_mut_val* activity_obj = yyjson_mut_obj(doc);
        yyjson_mut_obj_add_int(doc, activity_obj, "logical_id", activity_data.logical_id);
        yyjson_mut_obj_add_int(doc, activity_obj, "start_timestamp", activity_data.start_timestamp);
        yyjson_mut_obj_add_int(doc, activity_obj, "end_timestamp", activity_data.end_timestamp);
        yyjson_mut_obj_add_str(doc, activity_obj, "start_time", activity_data.start_time_str.c_str());
        yyjson_mut_obj_add_str(doc, activity_obj, "end_time", activity_data.end_time_str.c_str());
        yyjson_mut_obj_add_int(doc, activity_obj, "duration_seconds", activity_data.duration_seconds);

        if (activity_data.remark.has_value()) {
            yyjson_mut_obj_add_str(doc, activity_obj, "activity_remark", activity_data.remark.value().c_str());
        } else {
            yyjson_mut_obj_add_null(doc, activity_obj, "activity_remark");
        }

        yyjson_mut_val* activity_details = yyjson_mut_obj(doc);
        yyjson_mut_obj_add_str(doc, activity_details, "project_path", activity_data.project_path.c_str());
        yyjson_mut_obj_add_val(doc, activity_obj, "activity", activity_details);
        
        yyjson_mut_arr_add_val(activities, activity_obj);
    }
    yyjson_mut_obj_add_val(doc, day_obj, "activities", activities);

    // 3. Generated Stats
    yyjson_mut_val* stats_obj = yyjson_mut_obj(doc);
    yyjson_mut_obj_add_int(doc, stats_obj, "sleep_night_time", day.stats.sleep_night_time);
    yyjson_mut_obj_add_int(doc, stats_obj, "sleep_day_time", day.stats.sleep_day_time);
    yyjson_mut_obj_add_int(doc, stats_obj, "sleep_total_time", day.stats.sleep_total_time);
    yyjson_mut_obj_add_int(doc, stats_obj, "total_exercise_time", day.stats.total_exercise_time);
    yyjson_mut_obj_add_int(doc, stats_obj, "cardio_time", day.stats.cardio_time);
    yyjson_mut_obj_add_int(doc, stats_obj, "anaerobic_time", day.stats.anaerobic_time);
    yyjson_mut_obj_add_int(doc, stats_obj, "grooming_time", day.stats.grooming_time);
    yyjson_mut_obj_add_int(doc, stats_obj, "toilet_time", day.stats.toilet_time);
    yyjson_mut_obj_add_int(doc, stats_obj, "gaming_time", day.stats.gaming_time);
    yyjson_mut_obj_add_int(doc, stats_obj, "recreation_time", day.stats.recreation_time);
    yyjson_mut_obj_add_int(doc, stats_obj, "recreation_zhihu_time", day.stats.recreation_zhihu_time);
    yyjson_mut_obj_add_int(doc, stats_obj, "recreation_bilibili_time", day.stats.recreation_bilibili_time);
    yyjson_mut_obj_add_int(doc, stats_obj, "recreation_douyin_time", day.stats.recreation_douyin_time);
    yyjson_mut_obj_add_int(doc, stats_obj, "total_study_time", day.stats.study_time);
    
    yyjson_mut_obj_add_val(doc, day_obj, "generated_stats", stats_obj);

    return day_obj;
}

} // namespace serializer::core