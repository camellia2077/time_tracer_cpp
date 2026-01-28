// domain/model/generation_context.hpp
// 领域模型：生成上下文（聚合根）
#pragma once
#ifndef DOMAIN_MODEL_GENERATION_CONTEXT_H_
#define DOMAIN_MODEL_GENERATION_CONTEXT_H_

#include "domain/model/config.hpp"
#include "domain/model/remark_config.hpp"
#include <vector>
#include <string>
#include <optional>

namespace domain::model {

/**
 * @struct GenerationContext
 * @brief 日志生成的完整上下文，包含运行所需的全部配置数据
 * 
 * 这是一个聚合根，将所有生成相关的配置聚合在一起。
 * 由 Application 层的 ConfigHandler 构建，供 WorkflowHandler 和 Generator 使用。
 */
struct GenerationContext {
    Config config;                                          // 核心运行时配置
    std::vector<std::string> all_activities;                // 所有可用的活动名称
    std::optional<DailyRemarkConfig> remarks;               // 每日备注配置（可选）
    std::optional<ActivityRemarkConfig> activity_remarks;   // 活动备注配置（可选）
    std::vector<std::string> wake_keywords;                 // 起床关键词列表
};

}  // namespace domain::model

#endif  // DOMAIN_MODEL_GENERATION_CONTEXT_H_
