// application/steps/logic_linker_step.hpp
#ifndef APPLICATION_STEPS_LOGIC_LINKER_STEP_HPP_
#define APPLICATION_STEPS_LOGIC_LINKER_STEP_HPP_

#include "application/pipeline/interfaces/i_pipeline_step.hpp"

namespace core::pipeline {

class LogicLinkerStep : public IPipelineStep {
public:
  bool Execute(PipelineContext &context) override;
  std::string GetName() const override { return "LogicLinker"; }
};

} // namespace core::pipeline
#endif
