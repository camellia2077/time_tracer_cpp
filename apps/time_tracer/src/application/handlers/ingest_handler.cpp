// application/handlers/ingest_handler.cpp
// 入库处理器实�?- 内部调用现有 Pipeline
#include "application/handlers/ingest_handler.hpp"
#include "application/pipeline_factory.hpp"
#include "application/pipeline/context/pipeline_context.hpp"
#include "core/infrastructure/services/import_service.hpp"
#include <stdexcept>

namespace core::handlers {

IngestHandler::IngestHandler(
    const AppConfig& config,
    const fs::path& output_root,
    std::shared_ptr<core::interfaces::IFileSystem> file_system,
    std::shared_ptr<core::interfaces::IUserNotifier> notifier,
    std::shared_ptr<core::interfaces::ILogConverter> converter,
    std::shared_ptr<core::interfaces::ILogSerializer> serializer
)
    : config_(config),
      output_root_(output_root),
      file_system_(std::move(file_system)),
      notifier_(std::move(notifier)),
      converter_(std::move(converter)),
      serializer_(std::move(serializer))
{}

IngestHandler::~IngestHandler() = default;

core::commands::IngestResult IngestHandler::handle(const core::commands::IngestCommand& command) {
    core::commands::IngestResult result;
    
    notifier_->notify_info("\n--- 启动数据摄入 (Ingest) ---");
    
    try {
        // 构建 AppOptions (�?Command 转换)
        AppOptions options;
        options.validate_structure = true;
        options.convert = true;
        options.validate_logic = true;
        options.date_check_mode = command.date_check_mode;
        options.save_processed_output = command.save_processed_output;
        
        // 创建 Pipeline Context
        core::pipeline::PipelineContext context(config_, output_root_, file_system_, notifier_);
        context.config.input_root = command.source_path;
        context.config.date_check_mode = command.date_check_mode;
        context.config.save_processed_output = command.save_processed_output;
        
        // 调用现有 Pipeline
        auto pipeline = core::pipeline::PipelineFactory::create_ingest_pipeline(
            options, config_, serializer_, converter_
        );
        
        auto result_context_opt = pipeline->run(std::move(context));
        
        if (result_context_opt) {
            const auto& pipeline_result = result_context_opt->result;
            
            // 转换 Pipeline 结果�?Command 结果
            result.success = true;
            result.daily_logs = pipeline_result.processed_data;
            result.processed_files = result_context_opt->state.source_files;
            
            notifier_->notify_success("\n=== Ingest 执行成功 ===");
        } else {
            result.success = false;
            result.errors.push_back("Pipeline execution failed");
            notifier_->notify_error("\n=== Ingest 执行失败 ===");
        }
        
    } catch (const std::exception& e) {
        result.success = false;
        result.errors.push_back(e.what());
        notifier_->notify_error("=== Ingest 执行失败: " + std::string(e.what()) + " ===");
    }
    
    return result;
}

} // namespace core::handlers
