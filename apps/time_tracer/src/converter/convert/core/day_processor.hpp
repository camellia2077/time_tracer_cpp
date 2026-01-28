// converter/convert/core/day_processor.hpp
#ifndef CONVERTER_CONVERT_CORE_DAY_PROCESSOR_HPP_
#define CONVERTER_CONVERT_CORE_DAY_PROCESSOR_HPP_

#include "common/config/models/converter_config_models.hpp"
#include "core/domain/model/daily_log.hpp"

class DayProcessor {
public:
  // [Fix] 类型重命名: MapperConfig -> LogMapperConfig
  explicit DayProcessor(const LogMapperConfig &config);
  void Process(DailyLog &previousDay, DailyLog &dayToProcess);

private:
  const LogMapperConfig &config_;
};

#endif // CONVERTER_CONVERT_CORE_DAY_PROCESSOR_HPP_
