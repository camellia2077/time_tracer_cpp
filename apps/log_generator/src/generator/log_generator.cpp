// generator/log_generator.cpp
// 生成器模块：日志生成器实现
#include "generator/log_generator.hpp"
#include <format>
#include <iterator>

namespace generator {

LogGenerator::LogGenerator(
    const domain::model::Config& config,
    const std::vector<std::string>& activities,
    const std::optional<domain::model::DailyRemarkConfig>& remark_config,
    const std::optional<domain::model::ActivityRemarkConfig>& activity_remark_config,
    const std::vector<std::string>& wake_keywords)
    : gen_(std::random_device{}())
{
    day_generator_ = std::make_unique<DayGenerator>(
        config.items_per_day, 
        activities, 
        remark_config, 
        activity_remark_config, 
        wake_keywords, 
        gen_
    );
    
    sleep_scheduler_ = std::make_unique<SleepScheduler>(config.enable_nosleep, gen_);
}

void LogGenerator::GenerateForMonth(int year, int month, int days_in_month, std::string& buffer) {
    buffer.clear();
    
    size_t needed_capacity = (days_in_month + 1) * 300;
    if (buffer.capacity() < needed_capacity) {
        buffer.reserve(needed_capacity);
    }

    std::format_to(std::back_inserter(buffer), "y{}\n\n", year);

    sleep_scheduler_->ResetForNewMonth();

    bool is_nosleep = sleep_scheduler_->DetermineIfNosleep(1, days_in_month);
    day_generator_->GenerateForDay(buffer, month, 1, is_nosleep);

    for (int day = 2; day <= days_in_month; ++day) {
        buffer.push_back('\n');
        is_nosleep = sleep_scheduler_->DetermineIfNosleep(day, days_in_month);
        day_generator_->GenerateForDay(buffer, month, day, is_nosleep);
    }
}

}  // namespace generator
