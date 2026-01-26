// core/application/pipeline_factory.hpp
#ifndef CORE_APPLICATION_PIPELINE_FACTORY_HPP_
#define CORE_APPLICATION_PIPELINE_FACTORY_HPP_

#include <memory>
#include "core/application/pipeline_runner.hpp"
#include "common/app_options.hpp"
#include "common/config/app_config.hpp"
#include "core/application/interfaces/i_log_serializer.hpp"
#include "core/application/interfaces/i_log_converter.hpp" // [新增]

namespace core::pipeline {

class PipelineFactory {
public:
    static std::unique_ptr<PipelineRunner> create_ingest_pipeline(
        const AppOptions& options, 
        const AppConfig& config,
        std::shared_ptr<core::interfaces::ILogSerializer> serializer,
        std::shared_ptr<core::interfaces::ILogConverter> converter // [新增]
    );
};

}
#endif