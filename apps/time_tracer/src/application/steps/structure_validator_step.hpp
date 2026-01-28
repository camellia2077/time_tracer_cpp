// application/steps/structure_validator_step.hpp
#ifndef APPLICATION_STEPS_STRUCTURE_VALIDATOR_STEP_HPP_
#define APPLICATION_STEPS_STRUCTURE_VALIDATOR_STEP_HPP_

#include "application/pipeline/interfaces/i_pipeline_step.hpp"

namespace core::pipeline {

class StructureValidatorStep : public IPipelineStep {
public:
  bool Execute(PipelineContext &context) override;
  std::string GetName() const override { return "StructureValidator"; }
};

} // namespace core::pipeline
#endif
