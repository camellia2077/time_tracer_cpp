// importer/model/time_sheet_data.hpp
#ifndef IMPORTER_MODEL_TIME_SHEET_DATA_HPP_
#define IMPORTER_MODEL_TIME_SHEET_DATA_HPP_

#include <optional>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

// [修复] 引用路径修正：指向 Core Domain 层的新位置
#include "core/domain/model/time_data_models.hpp"

// --- Data Structures ---

/**
 * @struct DayData
 * @brief Holds metadata for a single day.
 */
struct DayData {
  std::string date_;
  std::string remark_;
  std::string getup_time_;

  int year_ = 0;
  int month_ = 0;
  int status_ = 0;
  int sleep_ = 0;
  int exercise_ = 0;

  // 统计数据聚合到 stats 中
  ActivityStats stats_;
};

/**
 * @struct TimeRecordInternal
 * @brief Holds data for a single time-logged activity.
 */
// 继承 BaseActivityRecord 复用字段
struct TimeRecordInternal : public BaseActivityRecord {
  // BaseActivityRecord 包含:
  // logical_id_, start_timestamp_, end_timestamp_,
  // start_time_str_, end_time_str_, project_path_,
  // duration_seconds_, remark_

  // Importer 特有的附加字段
  std::string date_; // 用于外键关联
};

/**
 * @struct pair_hash
 * @brief A custom hash function for using std::pair in an unordered_set.
 */
struct pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &p) const {
    auto h1 = std::hash<T1>{}(p.first);
    auto h2 = std::hash<T2>{}(p.second);
    return h1 ^ (h2 << 1);
  }
};

#endif // IMPORTER_MODEL_TIME_SHEET_DATA_HPP_
