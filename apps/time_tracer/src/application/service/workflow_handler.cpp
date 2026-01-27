// application/service/workflow_handler.cpp
#include "application/service/workflow_handler.hpp"
#include "application/pipeline_factory.hpp" 
#include "core/infrastructure/persistence/db_manager.hpp"
// [移除] #include "serializer/json_serializer.hpp"  <-- 不需要了，只依赖接口
// [移除] #include "converter/log_processor.hpp"     <-- 不需要了，只依赖接口
#include <stdexcept>

// [修改] 构造函数实现：接收并保存所有依�?
WorkflowHandler::WorkflowHandler(const std::string& db_path, 
                                 const AppConfig& config, 
                                 const fs::path& output_root_path, 
                                 std::shared_ptr<core::interfaces::IFileSystem> fs,
                                 std::shared_ptr<core::interfaces::IUserNotifier> notifier,
                                 std::shared_ptr<core::interfaces::ILogSerializer> serializer,
                                 std::shared_ptr<core::interfaces::ILogConverter> converter)
    : app_config_(config),
      output_root_path_(output_root_path),
      fs_(fs),
      notifier_(notifier),
      serializer_(serializer), // 保存
      converter_(converter)    // 保存
{
    // [修改] 创建 ImportService 时，传递已经注入进来的 serializer_
    import_service_ = std::make_unique<core::service::ImportService>(db_path, fs, notifier, serializer_);
}

WorkflowHandler::~WorkflowHandler() = default;

void WorkflowHandler::run_converter(const std::string& input_path, const AppOptions& options) {
    core::pipeline::PipelineContext context(app_config_, output_root_path_, fs_, notifier_);
    context.config.input_root = input_path;
    context.config.date_check_mode = options.date_check_mode;
    context.config.save_processed_output = options.save_processed_output;

    // [修改] 将保存的 serializer_ �?converter_ 传递给 PipelineFactory
    auto pipeline = core::pipeline::PipelineFactory::create_ingest_pipeline(
        options, 
        app_config_, 
        serializer_, 
        converter_
    );
    
    pipeline->run(std::move(context));
}

const AppConfig& WorkflowHandler::get_config() const {
    return app_config_;
}

void WorkflowHandler::run_database_import(const std::string& processed_path_str) {
    import_service_->import_from_files(processed_path_str);
}

void WorkflowHandler::run_database_import_from_memory(const std::map<std::string, std::vector<DailyLog>>& data_map) {
    import_service_->import_from_memory(data_map);
}

void WorkflowHandler::run_ingest(const std::string& source_path, DateCheckMode date_check_mode, bool save_processed) {
    notifier_->notify_info("\n--- 启动数据摄入 (Ingest) ---");
    
    AppOptions full_options;
    full_options.validate_structure = true;
    full_options.convert = true;
    full_options.validate_logic = true;
    full_options.date_check_mode = date_check_mode;
    full_options.save_processed_output = save_processed;

    core::pipeline::PipelineContext context(app_config_, output_root_path_, fs_, notifier_);
    context.config.input_root = source_path;
    context.config.date_check_mode = date_check_mode;
    context.config.save_processed_output = save_processed;

    // [修改] 传递依赖给 PipelineFactory
    auto pipeline = core::pipeline::PipelineFactory::create_ingest_pipeline(
        full_options, 
        app_config_, 
        serializer_, 
        converter_
    );

    auto result_context_opt = pipeline->run(std::move(context));

    if (result_context_opt) {
        const auto& result = result_context_opt->result;
        if (!result.processed_data.empty()) {
            notifier_->notify_info("\n--- 流水线验证通过，准备入�?---");
            import_service_->import_from_memory(result.processed_data);
            notifier_->notify_success("\n=== Ingest 执行成功 ===");
        } else {
             notifier_->notify_warning("\n=== Ingest 完成但无数据产生 ===");
        }
    } else {
        notifier_->notify_error("\n=== Ingest 执行失败 ===");
        throw std::runtime_error("Ingestion process failed.");
    }
}