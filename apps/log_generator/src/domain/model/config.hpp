// domain/model/config.hpp
// 领域模型：运行时配置与生成模式
#pragma once
#ifndef DOMAIN_MODEL_CONFIG_H_
#define DOMAIN_MODEL_CONFIG_H_

namespace domain::model {

/**
 * @enum GenerationMode
 * @brief 日志生成模式：单年或年份范围
 */
enum class GenerationMode {
    kYearRange,   // 生成多年范围
    kSingleYear   // 生成单一年份
};

/**
 * @struct Config
 * @brief 核心运行时配置，控制生成行为
 * 
 * 这是应用程序的核心配置值对象，由 CLI 参数和配置文件合并而成。
 */
struct Config {
    int start_year;         // 起始年份
    int end_year;           // 结束年份
    int items_per_day;      // 每天生成的事件数量
    GenerationMode mode;    // 生成模式
    bool enable_nosleep = false;  // 是否启用通宵日（无睡眠）
};

}  // namespace domain::model

#endif  // DOMAIN_MODEL_CONFIG_H_
