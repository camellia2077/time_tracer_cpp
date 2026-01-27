// application/pipeline_runner.cpp
#include "application/pipeline_runner.hpp"

namespace core::pipeline {

void PipelineRunner::add_step(std::unique_ptr<IPipelineStep> step) {
    steps_.push_back(std::move(step));
}

std::optional<PipelineContext> PipelineRunner::run(PipelineContext context) {
    context.notifier->notify_info("\n=== Pipeline Start ===");
    
    for (const auto& step : steps_) {
        // 可在此处添加通用的耗时监控或日�?
        // context.notifier->notify_info("Running step: " + step->get_name());
        
        if (!step->execute(context)) {
            context.notifier->notify_error("Pipeline stopped at step: " + step->get_name());
            return std::nullopt;
        }
    }
    
    context.notifier->notify_info("=== Pipeline Success ===");
    return context;
}

}