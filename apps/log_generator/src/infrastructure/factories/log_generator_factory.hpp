// infrastructure/factories/log_generator_factory.hpp
// 基础设施层：日志生成器工厂实现
#pragma once
#ifndef INFRASTRUCTURE_FACTORIES_LOG_GENERATOR_FACTORY_H_
#define INFRASTRUCTURE_FACTORIES_LOG_GENERATOR_FACTORY_H_

#include "application/factories/log_generator_factory_interface.hpp"
#include "generator/log_generator.hpp"

namespace infrastructure::factories {

/**
 * @class LogGeneratorFactory
 * @brief 日志生成器工厂的具体实现
 * 
 * 实现 application::factories::LogGeneratorFactoryInterface 接口。
 * 根据 GenerationContext 创建具体的 LogGenerator 实例。
 */
class LogGeneratorFactory : public application::factories::LogGeneratorFactoryInterface {
public:
    std::unique_ptr<domain::ports::LogGeneratorInterface> Create(
        const domain::model::GenerationContext& context
    ) override {
        return std::make_unique<generator::LogGenerator>(
            context.config,
            context.all_activities,
            context.remarks,
            context.activity_remarks,
            context.wake_keywords
        );
    }
};

}  // namespace infrastructure::factories

#endif  // INFRASTRUCTURE_FACTORIES_LOG_GENERATOR_FACTORY_H_
