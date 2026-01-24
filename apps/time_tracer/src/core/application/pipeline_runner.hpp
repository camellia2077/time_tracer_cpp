// core/application/pipeline_runner.hpp
#ifndef CORE_APPLICATION_PIPELINE_RUNNER_HPP_
#define CORE_APPLICATION_PIPELINE_RUNNER_HPP_

#include <vector>
#include <memory>
#include <optional>
#include "core/application/pipeline/interfaces/i_pipeline_step.hpp"

namespace core::pipeline {

class PipelineRunner {
public:
    // 添加一个步骤到流水线
    void add_step(std::unique_ptr<IPipelineStep> step);

    // 运行流水线
    std::optional<PipelineContext> run(PipelineContext context);

private:
    std::vector<std::unique_ptr<IPipelineStep>> steps_;
};

} 
#endif