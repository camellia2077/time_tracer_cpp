// core/application/pipeline_factory.cpp
#include "core/application/pipeline_factory.hpp"

// 引入所有具体的步骤
#include "core/application/steps/file_collector.hpp"
#include "core/application/steps/config_loader_step.hpp"
#include "core/application/steps/structure_validator_step.hpp"
#include "core/application/steps/converter_step.hpp"
#include "core/application/steps/logic_linker_step.hpp"
#include "core/application/steps/logic_validator_step.hpp"
#include "core/application/steps/processed_data_writer_step.hpp" 

namespace core::pipeline {

std::unique_ptr<PipelineRunner> PipelineFactory::create_ingest_pipeline(
    const AppOptions& options, 
    [[maybe_unused]] const AppConfig& config, // [修复] 标记为可能不使用，消除编译错误
    std::shared_ptr<core::interfaces::ILogSerializer> serializer,
    std::shared_ptr<core::interfaces::ILogConverter> converter
) {
    auto runner = std::make_unique<PipelineRunner>();

    // 1. 总是执行收集
    runner->add_step(std::make_unique<FileCollector>());

    // 2. 加载配置
    if (options.validate_structure || options.convert || options.validate_logic) {
        runner->add_step(std::make_unique<ConfigLoaderStep>());
    }

    // 3. 结构验证
    if (options.validate_structure) {
        runner->add_step(std::make_unique<StructureValidatorStep>());
    }

    // 4. 转换与链接
    if (options.convert) {
        runner->add_step(std::make_unique<ConverterStep>(converter));
        runner->add_step(std::make_unique<LogicLinkerStep>());
    }

    // 5. 逻辑验证
    if (options.validate_logic) {
        runner->add_step(std::make_unique<LogicValidatorStep>());
    }

    // 6. 保存输出
    if (options.convert && options.save_processed_output) {
        runner->add_step(std::make_unique<ProcessedDataWriterStep>(serializer));
    }

    return runner;
}

}