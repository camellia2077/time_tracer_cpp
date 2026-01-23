// generator/impl/log_generator.hpp
#ifndef GENERATOR_IMPL_LOG_GENERATOR_HPP_
#define GENERATOR_IMPL_LOG_GENERATOR_HPP_

#include "generator/api/i_log_Generator.hpp"
#include "config/config.hpp"
#include "generator/components/day_generator.hpp"
#include "generator/strategies/sleep_scheduler.hpp" 
#include <vector>
#include <string>
#include <optional>
#include <random>
#include <memory>

class LogGenerator : public ILogGenerator {
public:
    LogGenerator(const Config& config,
                 const std::vector<std::string>& activities,
                 const std::optional<DailyRemarkConfig>& remark_config,
                 const std::optional<ActivityRemarkConfig>& activity_remark_config,
                 const std::vector<std::string>& wake_keywords);

    // 接口保持不变，支持内存复用
    void generate_for_month(int year, int month, int days_in_month, std::string& buffer) override;

private:
    std::mt19937 gen_;
    
    // 子组件：负责具体的单日内容生成
    std::unique_ptr<DayGenerator> day_generator_;

    // [新增] 子组件：负责通宵/睡眠调度策略
    // 所有的状态变量（enable_nosleep_, sequence_length 等）都移入了这个类
    std::unique_ptr<SleepScheduler> sleep_scheduler_;
};

#endif // GENERATOR_IMPL_LOG_GENERATOR_HPP_