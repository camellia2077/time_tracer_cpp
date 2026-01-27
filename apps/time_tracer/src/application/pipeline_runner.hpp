// application/pipeline_runner.hpp
#ifndef APPLICATION_PIPELINE_RUNNER_HPP_
#define APPLICATION_PIPELINE_RUNNER_HPP_

#include <vector>
#include <memory>
#include <optional>
#include "application/pipeline/interfaces/i_pipeline_step.hpp"

namespace core::pipeline {

class PipelineRunner {
public:
    // 添加一个步骤到流水�?
    void add_step(std::unique_ptr<IPipelineStep> step);

    // 运行流水�?
    std::optional<PipelineContext> run(PipelineContext context);

private:
    std::vector<std::unique_ptr<IPipelineStep>> steps_;
};

} 
#endif