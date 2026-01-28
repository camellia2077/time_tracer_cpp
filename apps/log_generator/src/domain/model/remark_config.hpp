// domain/model/remark_config.hpp
// 领域模型：备注配置
#pragma once
#ifndef DOMAIN_MODEL_REMARK_CONFIG_H_
#define DOMAIN_MODEL_REMARK_CONFIG_H_

#include <string>
#include <vector>

namespace domain::model {

/**
 * @struct DailyRemarkConfig
 * @brief 每日备注的配置
 * 
 * 控制日志中每天头部备注的生成规则。
 */
struct DailyRemarkConfig {
    std::string prefix;                     // 备注前缀
    std::vector<std::string> contents;      // 备注内容池
    double generation_chance = 0.5;         // 生成概率 (0.0-1.0)
    int max_lines = 1;                      // 最大行数
};

/**
 * @struct ActivityRemarkConfig
 * @brief 活动备注的配置
 * 
 * 控制事件行末尾备注的生成规则。
 */
struct ActivityRemarkConfig {
    std::vector<std::string> contents;      // 备注内容池
    double generation_chance = 0.5;         // 生成概率 (0.0-1.0)
};

}  // namespace domain::model

#endif  // DOMAIN_MODEL_REMARK_CONFIG_H_
