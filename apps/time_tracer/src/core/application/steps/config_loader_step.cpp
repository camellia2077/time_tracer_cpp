// core/application/steps/config_loader_step.cpp
#include "core/application/steps/config_loader_step.hpp"
#include "core/application/utils/converter_config_factory.hpp" 

namespace core::pipeline {

bool ConfigLoaderStep::execute(PipelineContext& context) {
    // 检查是否需要加载配置（避免重复加载，或者如果路径为空则跳过）
    if (context.config.app_config.pipeline.interval_processor_config_path.empty()) {
        context.notifier->notify_warning("Skipping config load: No interval processor config path provided.");
        return true;
    }

    context.notifier->notify_info("Step: Loading Converter Configuration...");

    try {
        // 调用工厂加载配置 (interval_processor_config.toml -> mapping_config.toml 等)
        context.state.converter_config = ConverterConfigFactory::create(
            *context.file_system,
            context.config.app_config.pipeline.interval_processor_config_path,
            context.config.app_config
        );
        return true;
    } catch (const std::exception& e) {
        context.notifier->notify_error("Failed to load configuration: " + std::string(e.what()));
        return false;
    }
}

}