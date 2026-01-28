// converter/convert/core/log_linker.hpp
#ifndef CONVERTER_CONVERT_CORE_LOG_LINKER_HPP_
#define CONVERTER_CONVERT_CORE_LOG_LINKER_HPP_

#include "common/config/models/converter_config_models.hpp"
#include "core/domain/model/daily_log.hpp"
#include <map>
#include <string>
#include <vector>

class LogLinker {
public:
  // [Fix] 类型重命名: LinkerConfig -> LogLinkerConfig
  explicit LogLinker(const LogLinkerConfig &config);
  void LinkLogs(std::map<std::string, std::vector<DailyLog>> &data_map);

private:
  const LogLinkerConfig &config_;
  void ProcessCrossDay(DailyLog &current_day, const DailyLog &prev_day);
  void RecalculateStats(DailyLog &day);
};

#endif // CONVERTER_CONVERT_CORE_LOG_LINKER_HPP_
