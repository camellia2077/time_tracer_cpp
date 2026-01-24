// core/application/pipeline_factory.hpp
#ifndef CORE_APPLICATION_PIPELINE_FACTORY_HPP_
#define CORE_APPLICATION_PIPELINE_FACTORY_HPP_

#include <memory>
#include "core/application/pipeline_runner.hpp"
#include "common/app_options.hpp"
#include "common/config/app_config.hpp"

namespace core::pipeline {

class PipelineFactory {
public:
    static std::unique_ptr<PipelineRunner> create_ingest_pipeline(
        const AppOptions& options, 
        const AppConfig& config
    );
};

}
#endif