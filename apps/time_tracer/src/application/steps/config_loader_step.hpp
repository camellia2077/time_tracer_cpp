// application/steps/config_loader_step.hpp
#ifndef APPLICATION_STEPS_CONFIG_LOADER_STEP_HPP_
#define APPLICATION_STEPS_CONFIG_LOADER_STEP_HPP_

#include "application/pipeline/interfaces/i_pipeline_step.hpp"

namespace core::pipeline {

class ConfigLoaderStep : public IPipelineStep {
public:
    bool execute(PipelineContext& context) override;
    std::string get_name() const override { return "ConfigLoader"; }
};

} 
#endif