// application/pipeline/factory.hpp
#ifndef APPLICATION_PIPELINE_FACTORY_HPP_
#define APPLICATION_PIPELINE_FACTORY_HPP_

#include "application/interfaces/i_log_converter.hpp" // [新增]
#include "application/interfaces/i_log_serializer.hpp"
#include "application/pipeline/runner.hpp"
#include "common/app_options.hpp"
#include "common/config/app_config.hpp"
#include <memory>

namespace core::pipeline {

class PipelineFactory {
public:
  static std::unique_ptr<PipelineRunner> CreateIngestPipeline(
      const AppOptions &options, const AppConfig &config,
      std::shared_ptr<core::interfaces::ILogSerializer> serializer,
      std::shared_ptr<core::interfaces::ILogConverter> converter // [新增]
  );
};

} // namespace core::pipeline
#endif
