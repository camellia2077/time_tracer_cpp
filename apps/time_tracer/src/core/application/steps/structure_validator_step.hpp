// core/application/steps/structure_validator_step.hpp
#ifndef CORE_APPLICATION_STEPS_STRUCTURE_VALIDATOR_STEP_HPP_
#define CORE_APPLICATION_STEPS_STRUCTURE_VALIDATOR_STEP_HPP_

#include "core/application/pipeline/interfaces/i_pipeline_step.hpp"

namespace core::pipeline {

class StructureValidatorStep : public IPipelineStep {
public:
    bool execute(PipelineContext& context) override;
    std::string get_name() const override { return "StructureValidator"; }
};

} 
#endif