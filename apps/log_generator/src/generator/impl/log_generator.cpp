// generator/impl/log_generator.cpp
#include "generator/impl/log_generator.hpp"
#include <format>
#include <iterator>

LogGenerator::LogGenerator(const Config& config,
                         const std::vector<std::string>& activities,
                         const std::optional<DailyRemarkConfig>& remark_config,
                         const std::optional<ActivityRemarkConfig>& activity_remark_config,
                         const std::vector<std::string>& wake_keywords)
    : gen_(std::random_device{}()) // 初始化随机数引擎
{
    // 初始化 DayGenerator
    day_generator_ = std::make_unique<DayGenerator>(
        config.items_per_day, 
        activities, 
        remark_config, 
        activity_remark_config, 
        wake_keywords, 
        gen_
    );
    
    // 初始化 SleepScheduler，将策略逻辑委托给它
    sleep_scheduler_ = std::make_unique<SleepScheduler>(config.enable_nosleep, gen_);
}

void LogGenerator::generate_for_month(int year, int month, int days_in_month, std::string& buffer) {
    buffer.clear(); // 复用内存
    
    // 确保容量足够，避免频繁 realloc
    size_t needed_capacity = (days_in_month + 1) * 300;
    if (buffer.capacity() < needed_capacity) {
        buffer.reserve(needed_capacity);
    }

    // [修改] 已移除 1 月份生成上一年 12 月 31 日的特殊逻辑
    
    // 写入当前年份标题
    std::format_to(std::back_inserter(buffer), "y{}\n\n", year);

    // 1. 每个月开始前，重置调度器状态
    sleep_scheduler_->reset_for_new_month();

    // 2. 生成第 1 天
    // 通过调度器查询是否通宵
    bool is_nosleep = sleep_scheduler_->determine_if_nosleep(1, days_in_month);
    day_generator_->generate_for_day(buffer, month, 1, is_nosleep);

    // 3. 循环生成剩余天数
    for (int day = 2; day <= days_in_month; ++day) {
        buffer.push_back('\n');

        // 直接询问策略类
        is_nosleep = sleep_scheduler_->determine_if_nosleep(day, days_in_month);
        
        day_generator_->generate_for_day(buffer, month, day, is_nosleep);
    }
}