// application/steps/converter_step.hpp
#ifndef APPLICATION_STEPS_CONVERTER_STEP_HPP_
#define APPLICATION_STEPS_CONVERTER_STEP_HPP_

#include "application/interfaces/i_log_converter.hpp" // [新增]
#include "application/pipeline/interfaces/i_pipeline_step.hpp"
#include <memory>

// 前向声明
namespace core::interfaces {
class IUserNotifier;
}

namespace core::pipeline {

class ConverterStep : public IPipelineStep {
public:
  // [修改] 构造函数注�?ILogConverter
  explicit ConverterStep(
      std::shared_ptr<core::interfaces::ILogConverter> converter);

  bool Execute(PipelineContext &context) override;
  std::string GetName() const override { return "Converter"; }

private:
  std::shared_ptr<core::interfaces::ILogConverter> converter_; // [新增]
  void printTiming(
      double ms,
      const std::shared_ptr<core::interfaces::IUserNotifier> &notifier) const;
};

} // namespace core::pipeline
#endif
