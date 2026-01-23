// generator/api/log_generator_factory.hpp
#ifndef GENERATOR_API_LOG_GENERATOR_FACTORY_HPP_
#define GENERATOR_API_LOG_GENERATOR_FACTORY_HPP_

#include "generator/api/i_log_generator.hpp"
#include "common/app_context.hpp"
#include "generator/impl/log_generator.hpp" 
#include <memory>

class ILogGeneratorFactory {
public:
    virtual ~ILogGeneratorFactory() = default;
    virtual std::unique_ptr<ILogGenerator> create(const Core::AppContext& context) = 0;
};

class LogGeneratorFactory : public ILogGeneratorFactory {
public:
    std::unique_ptr<ILogGenerator> create(const Core::AppContext& context) override {
        return std::make_unique<LogGenerator>(
            context.config,
            context.all_activities,
            context.remarks,
            context.activity_remarks,
            context.wake_keywords
        );
    }
};

#endif // GENERATOR_API_LOG_GENERATOR_FACTORY_HPP_