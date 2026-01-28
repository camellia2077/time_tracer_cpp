// application/pipeline/interfaces/i_pipeline_step.hpp
#pragma once
#include "application/pipeline/context/pipeline_context.hpp"

namespace core::pipeline {

class IPipelineStep {
public:
  virtual ~IPipelineStep() = default;
  // 返回 false 表示流程应当终止
  virtual bool Execute(PipelineContext &context) = 0;
  virtual std::string GetName() const = 0;
};

} // namespace core::pipeline
