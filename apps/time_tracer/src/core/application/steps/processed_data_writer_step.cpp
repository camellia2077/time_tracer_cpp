// core/application/steps/processed_data_writer_step.cpp
#include "core/application/steps/processed_data_writer_step.hpp"
#include "core/application/utils/processed_data_writer.hpp"

namespace core::pipeline {

bool ProcessedDataWriterStep::execute(PipelineContext& context) {
    context.notifier->notify_info("Step: Saving Validated JSON...");

    auto new_files = ProcessedDataWriter::write(
        context.result.processed_data,
        context.cached_json_outputs, 
        context.config.output_root,
        *context.file_system,
        *context.notifier
    );

    context.state.generated_files.insert(
        context.state.generated_files.end(),
        new_files.begin(), 
        new_files.end()
    );
    
    if (new_files.empty() && !context.result.processed_data.empty()) {
        context.notifier->notify_warning("[Pipeline] 警告: 数据存在但未生成文件 (可能写入失败)。");
    } else {
        context.notifier->notify_success("JSON 数据已安全落盘。");
    }

    return true; // 即使写入失败，通常也不中断流程，或者根据需求返回 false
}

}