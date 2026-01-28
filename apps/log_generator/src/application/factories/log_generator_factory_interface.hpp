// application/factories/log_generator_factory_interface.hpp
// 应用层：日志生成器工厂接口
#pragma once
#ifndef APPLICATION_FACTORIES_LOG_GENERATOR_FACTORY_INTERFACE_H_
#define APPLICATION_FACTORIES_LOG_GENERATOR_FACTORY_INTERFACE_H_

#include "domain/ports/log_generator_interface.hpp"
#include "domain/model/generation_context.hpp"
#include <memory>

namespace application::factories {

/**
 * @interface LogGeneratorFactoryInterface
 * @brief 日志生成器工厂接口
 * 
 * 这是一个抽象工厂接口，由 Application 层 define，Infrastructure 层实现。
 * 通过工厂模式实现依赖倒置：
 * - Application 层依赖此接口
 * - Infrastructure 层提供具体的 LogGeneratorFactory 实现
 */
class LogGeneratorFactoryInterface {
public:
    virtual ~LogGeneratorFactoryInterface() = default;

    /**
     * @brief 根据生成上下文创建日志生成器实例
     * @param context 生成上下文（包含配置和活动列表）
     * @return 日志生成器实例
     */
    virtual std::unique_ptr<domain::ports::LogGeneratorInterface> Create(
        const domain::model::GenerationContext& context
    ) = 0;
};

}  // namespace application::factories

#endif  // APPLICATION_FACTORIES_LOG_GENERATOR_FACTORY_INTERFACE_H_
