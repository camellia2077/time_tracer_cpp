// importer/parser/memory_parser.cpp
#include "importer/parser/memory_parser.hpp"
#include <iostream>

ParsedData MemoryParser::parse(
    const std::map<std::string, std::vector<DailyLog>> &data_map) {
  ParsedData all_data;

  for (const auto &[month_str, days] : data_map) {
    for (const auto &input_day : days) {
      DayData day_data;
      day_data.date_ = input_day.date_;

      if (input_day.date_.length() >= 7) {
        try {
          day_data.year_ = std::stoi(input_day.date_.substr(0, 4));
          day_data.month_ = std::stoi(input_day.date_.substr(5, 2));
        } catch (...) {
          day_data.year_ = 0;
          day_data.month_ = 0;
        }
      } else {
        day_data.year_ = 0;
        day_data.month_ = 0;
      }

      day_data.status_ = input_day.has_study_activity_ ? 1 : 0;
      day_data.sleep_ = input_day.has_sleep_activity_ ? 1 : 0;
      day_data.exercise_ = input_day.has_exercise_activity_ ? 1 : 0;
      day_data.getup_time_ =
          input_day.getup_time_.empty() ? "00:00" : input_day.getup_time_;

      std::string merged_remark;
      for (size_t i = 0; i < input_day.general_remarks_.size(); ++i) {
        merged_remark += input_day.general_remarks_[i];
        if (i < input_day.general_remarks_.size() - 1) {
          merged_remark += "\n";
        }
      }
      day_data.remark_ = merged_remark;

      // [核心修改] 直接赋值统计数据！
      day_data.stats_ = input_day.stats_;

      all_data.days.push_back(day_data);

      for (const auto &activity : input_day.processed_activities_) {
        TimeRecordInternal record;

        // [核心修改] 利用 BaseActivityRecord 的赋值
        // record 是 TimeRecordInternal，继承自 BaseActivityRecord
        // activity 也是 BaseActivityRecord
        static_cast<BaseActivityRecord &>(record) = activity;

        // 补充特有字段
        record.date_ = input_day.date_;

        all_data.records.push_back(record);
      }
    }
  }
  return all_data;
}
