// generator/log_generator.hpp
// 生成器模块：日志生成器实现
#pragma once
#ifndef GENERATOR_LOG_GENERATOR_H_
#define GENERATOR_LOG_GENERATOR_H_

#include "domain/ports/log_generator_interface.hpp"
#include "domain/model/config.hpp"
#include "domain/model/remark_config.hpp"
#include "generator/components/day_generator.hpp"
#include "generator/strategies/sleep_scheduler.hpp"
#include <vector>
#include <string>
#include <optional>
#include <random>
#include <memory>

namespace generator {

/**
 * @class LogGenerator
 * @brief 日志生成器实现，实现 domain::ports::LogGeneratorInterface 接口
 * 
 * 职责：
 * - 协调子组件生成完整的月度日志
 * - 管理随机数引擎（线程安全考虑）
 */
class LogGenerator : public domain::ports::LogGeneratorInterface {
public:
    LogGenerator(
        const domain::model::Config& config,
        const std::vector<std::string>& activities,
        const std::optional<domain::model::DailyRemarkConfig>& remark_config,
        const std::optional<domain::model::ActivityRemarkConfig>& activity_remark_config,
        const std::vector<std::string>& wake_keywords
    );

    void GenerateForMonth(int year, int month, int days_in_month, std::string& buffer) override;

private:
    std::mt19937 gen_;
    std::unique_ptr<DayGenerator> day_generator_;
    std::unique_ptr<SleepScheduler> sleep_scheduler_;
};

}  // namespace generator

#endif  // GENERATOR_LOG_GENERATOR_H_
