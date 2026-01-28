// converter/convert/core/activity_mapper.hpp
#ifndef CONVERTER_CONVERT_CORE_ACTIVITY_MAPPER_HPP_
#define CONVERTER_CONVERT_CORE_ACTIVITY_MAPPER_HPP_

#include "common/config/models/converter_config_models.hpp"
#include "core/domain/model/daily_log.hpp"

class ActivityMapper {
public:
  // [Fix] 类型重命名: MapperConfig -> LogMapperConfig
  explicit ActivityMapper(const LogMapperConfig &config);
  void MapActivities(DailyLog &day);

private:
  const LogMapperConfig &config_;
};

#endif // CONVERTER_CONVERT_CORE_ACTIVITY_MAPPER_HPP_
