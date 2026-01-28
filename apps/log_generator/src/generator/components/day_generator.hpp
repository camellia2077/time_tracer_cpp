// generator/components/day_generator.hpp
#pragma once
#ifndef GENERATOR_COMPONENTS_DAY_GENERATOR_H_
#define GENERATOR_COMPONENTS_DAY_GENERATOR_H_

#include "generator/components/remark_generator.hpp"
#include "generator/components/event_generator.hpp"
#include "domain/model/remark_config.hpp"
#include <string>
#include <memory>
#include <vector>
#include <optional>
#include <random>

namespace generator {

/**
 * @class DayGenerator
 * @brief (内部组件) 专门负责生成一天完整的日志内容。
 */
class DayGenerator {
public:
    DayGenerator(int items_per_day,
                 const std::vector<std::string>& activities,
                 const std::optional<domain::model::DailyRemarkConfig>& remark_config,
                 const std::optional<domain::model::ActivityRemarkConfig>& activity_remark_config,
                 const std::vector<std::string>& wake_keywords,
                 std::mt19937& gen);

    void GenerateForDay(std::string& log_content, int month, int day, bool is_nosleep_day);

private:
    std::unique_ptr<RemarkGenerator> remark_generator_;
    std::unique_ptr<EventGenerator> event_generator_;
};

}  // namespace generator

#endif  // GENERATOR_COMPONENTS_DAY_GENERATOR_H_
