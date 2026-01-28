// application/pipeline/runner.hpp
#ifndef APPLICATION_PIPELINE_RUNNER_HPP_
#define APPLICATION_PIPELINE_RUNNER_HPP_

#include "application/pipeline/interfaces/i_pipeline_step.hpp"
#include <memory>
#include <optional>
#include <vector>

namespace core::pipeline {

class PipelineRunner {
public:
  // 添加一个步骤到流水线
  void AddStep(std::unique_ptr<IPipelineStep> step);

  // 运行流水线
  std::optional<PipelineContext> Run(PipelineContext context);

private:
  std::vector<std::unique_ptr<IPipelineStep>> steps_;
};

} // namespace core::pipeline
#endif
