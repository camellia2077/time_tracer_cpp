// core/application/steps/converter_step.hpp
#ifndef CORE_APPLICATION_STEPS_CONVERTER_STEP_HPP_
#define CORE_APPLICATION_STEPS_CONVERTER_STEP_HPP_

#include "core/application/pipeline/interfaces/i_pipeline_step.hpp"
#include "core/application/interfaces/i_log_converter.hpp" // [新增]
#include <memory>

// 前向声明
namespace core::interfaces { class IUserNotifier; }

namespace core::pipeline {

class ConverterStep : public IPipelineStep {
public:
    // [修改] 构造函数注入 ILogConverter
    explicit ConverterStep(std::shared_ptr<core::interfaces::ILogConverter> converter);
    
    bool execute(PipelineContext& context) override;
    std::string get_name() const override { return "Converter"; }

private:
    std::shared_ptr<core::interfaces::ILogConverter> converter_; // [新增]
    void printTiming(double ms, const std::shared_ptr<core::interfaces::IUserNotifier>& notifier) const;
};

}
#endif