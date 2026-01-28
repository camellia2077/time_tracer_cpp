// application/pipeline/factory.cpp
#include "application/pipeline/factory.hpp"

// 引入所有具体的步骤
#include "application/steps/config_loader_step.hpp"
#include "application/steps/converter_step.hpp"
#include "application/steps/file_collector.hpp"
#include "application/steps/logic_linker_step.hpp"
#include "application/steps/logic_validator_step.hpp"
#include "application/steps/processed_data_writer_step.hpp"
#include "application/steps/structure_validator_step.hpp"

namespace core::pipeline {

std::unique_ptr<PipelineRunner> PipelineFactory::CreateIngestPipeline(
    const AppOptions &options,
    [[maybe_unused]] const AppConfig
        &config, // [修复] 标记为可能不使用，消除编译错?
    std::shared_ptr<core::interfaces::ILogSerializer> serializer,
    std::shared_ptr<core::interfaces::ILogConverter> converter) {
  auto runner = std::make_unique<PipelineRunner>();

  // 1. 总是执行收集
  runner->AddStep(std::make_unique<FileCollector>());

  // 2. 加载配置
  if (options.validate_structure_ || options.convert_ ||
      options.validate_logic_) {
    runner->AddStep(std::make_unique<ConfigLoaderStep>());
  }

  // 3. 结构验证
  if (options.validate_structure_) {
    runner->AddStep(std::make_unique<StructureValidatorStep>());
  }

  // 4. 转换与链接
  if (options.convert_) {
    runner->AddStep(std::make_unique<ConverterStep>(converter));
    runner->AddStep(std::make_unique<LogicLinkerStep>());
  }

  // 5. 逻辑验证
  if (options.validate_logic_) {
    runner->AddStep(std::make_unique<LogicValidatorStep>());
  }

  // 6. 保存输出
  if (options.convert_ && options.save_processed_output_) {
    runner->AddStep(std::make_unique<ProcessedDataWriterStep>(serializer));
  }

  return runner;
}

} // namespace core::pipeline
