// core/application/steps/logic_validator_step.hpp
#ifndef CORE_APPLICATION_STEPS_LOGIC_VALIDATOR_STEP_HPP_
#define CORE_APPLICATION_STEPS_LOGIC_VALIDATOR_STEP_HPP_

#include "core/application/pipeline/interfaces/i_pipeline_step.hpp"

namespace core::pipeline {

class LogicValidatorStep : public IPipelineStep {
public:
    bool execute(PipelineContext& context) override;
    std::string get_name() const override { return "LogicValidator"; }
};

} 
#endif