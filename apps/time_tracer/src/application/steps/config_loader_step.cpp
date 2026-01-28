// application/steps/config_loader_step.cpp
#include "application/steps/config_loader_step.hpp"
#include "application/utils/converter_config_factory.hpp"

namespace core::pipeline {

bool ConfigLoaderStep::Execute(PipelineContext &context) {
  // 检查是否需要加载配置（避免重复加载，或者如果路径为空则跳过?
  if (context.config.app_config_.pipeline_.interval_processor_config_path_
          .empty()) {
    context.notifier->NotifyWarning(
        "Skipping config load: No interval processor config path provided.");
    return true;
  }

  context.notifier->NotifyInfo("Step: Loading Converter Configuration...");

  try {
    // 调用工厂加载配置 (interval_processor_config.toml -> mapping_config.toml ?
    context.state.converter_config = ConverterConfigFactory::Create(
        *context.file_system,
        context.config.app_config_.pipeline_.interval_processor_config_path_,
        context.config.app_config_);
    return true;
  } catch (const std::exception &e) {
    context.notifier->NotifyError("Failed to load configuration: " +
                                  std::string(e.what()));
    return false;
  }
}

} // namespace core::pipeline
