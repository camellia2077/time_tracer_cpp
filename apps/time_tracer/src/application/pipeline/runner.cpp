// application/pipeline/runner.cpp
#include "application/pipeline/runner.hpp"

namespace core::pipeline {

void PipelineRunner::AddStep(std::unique_ptr<IPipelineStep> step) {
  steps_.push_back(std::move(step));
}

std::optional<PipelineContext> PipelineRunner::Run(PipelineContext context) {
  context.notifier->NotifyInfo("\n=== Pipeline Start ===");

  for (const auto &step : steps_) {
    // 可在此处添加通用的耗时监控或日志
    // context.notifier->NotifyInfo("Running step: " + step->GetName());

    if (!step->Execute(context)) {
      context.notifier->NotifyError("Pipeline stopped at step: " +
                                    step->GetName());
      return std::nullopt;
    }
  }

  context.notifier->NotifyInfo("=== Pipeline Success ===");
  return context;
}

} // namespace core::pipeline
