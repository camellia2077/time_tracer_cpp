// core/application/steps/config_loader_step.hpp
#ifndef CORE_APPLICATION_STEPS_CONFIG_LOADER_STEP_HPP_
#define CORE_APPLICATION_STEPS_CONFIG_LOADER_STEP_HPP_

#include "core/application/pipeline/interfaces/i_pipeline_step.hpp"

namespace core::pipeline {

class ConfigLoaderStep : public IPipelineStep {
public:
    bool execute(PipelineContext& context) override;
    std::string get_name() const override { return "ConfigLoader"; }
};

} 
#endif