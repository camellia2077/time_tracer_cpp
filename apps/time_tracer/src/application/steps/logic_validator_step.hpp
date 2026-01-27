// application/steps/logic_validator_step.hpp
#ifndef APPLICATION_STEPS_LOGIC_VALIDATOR_STEP_HPP_
#define APPLICATION_STEPS_LOGIC_VALIDATOR_STEP_HPP_

#include "application/pipeline/interfaces/i_pipeline_step.hpp"

namespace core::pipeline {

class LogicValidatorStep : public IPipelineStep {
public:
    bool execute(PipelineContext& context) override;
    std::string get_name() const override { return "LogicValidator"; }
};

} 
#endif