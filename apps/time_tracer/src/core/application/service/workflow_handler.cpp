// core/application/service/workflow_handler.cpp
#include "core/application/service/workflow_handler.hpp"
#include "core/application/pipeline_factory.hpp" 
#include "core/infrastructure/persistence/db_manager.hpp"
#include <stdexcept>

WorkflowHandler::WorkflowHandler(const std::string& db_path, 
                                 const AppConfig& config, 
                                 const fs::path& output_root_path, 
                                 std::shared_ptr<core::interfaces::IFileSystem> fs,
                                 std::shared_ptr<core::interfaces::IUserNotifier> notifier)
    : app_config_(config),
      output_root_path_(output_root_path),
      fs_(fs),
      notifier_(notifier)
{
    // 初始化 ImportService
    import_service_ = std::make_unique<core::service::ImportService>(db_path, fs, notifier);
}

WorkflowHandler::~WorkflowHandler() = default;

void WorkflowHandler::run_converter(const std::string& input_path, const AppOptions& options) {
    // 组装 Context
    core::pipeline::PipelineContext context(app_config_, output_root_path_, fs_, notifier_);
    context.config.input_root = input_path;
    // 传递其他 options 到 context config 如果需要
    context.config.date_check_mode = options.date_check_mode;
    context.config.save_processed_output = options.save_processed_output;

    // 创建并运行 Pipeline
    auto pipeline = core::pipeline::PipelineFactory::create_ingest_pipeline(options, app_config_);
    pipeline->run(std::move(context));
}

const AppConfig& WorkflowHandler::get_config() const {
    return app_config_;
}

void WorkflowHandler::run_database_import(const std::string& processed_path_str) {
    // 委派给 Service
    import_service_->import_from_files(processed_path_str);
}

void WorkflowHandler::run_database_import_from_memory(const std::map<std::string, std::vector<DailyLog>>& data_map) {
    // 委派给 Service
    import_service_->import_from_memory(data_map);
}

void WorkflowHandler::run_ingest(const std::string& source_path, DateCheckMode date_check_mode, bool save_processed) {
    notifier_->notify_info("\n--- 启动数据摄入 (Ingest) ---");
    
    // DB 连接检查 (保留原有逻辑，但注意这里的异常处理)
    // 建议：这部分逻辑也应该移入 Validator Step 或者 ImportService 的预检查中
    try {
        // 假设 db_path_ 可以在这里通过某种方式访问，或者直接不做此检查，等待 ImportService 报错
        // 既然 ImportService 持有 db_path，可以增加一个 check_connection 方法
        // 这里暂时省略显式的 check，让 Import 阶段自然失败
    } catch (const std::exception& e) {
        notifier_->notify_error("DB Error: " + std::string(e.what()));
        throw;
    }

    // 1. 构建配置
    AppOptions full_options;
    full_options.validate_structure = true;
    full_options.convert = true;
    full_options.validate_logic = true;
    full_options.date_check_mode = date_check_mode;
    full_options.save_processed_output = save_processed;

    // 2. 准备上下文
    core::pipeline::PipelineContext context(app_config_, output_root_path_, fs_, notifier_);
    context.config.input_root = source_path;
    context.config.date_check_mode = date_check_mode;
    context.config.save_processed_output = save_processed;

    // 3. 工厂创建流水线
    auto pipeline = core::pipeline::PipelineFactory::create_ingest_pipeline(full_options, app_config_);

    // 4. 执行
    auto result_context_opt = pipeline->run(std::move(context));

    // 5. 处理结果
    if (result_context_opt) {
        const auto& result = result_context_opt->result;
        if (!result.processed_data.empty()) {
            notifier_->notify_info("\n--- 流水线验证通过，准备入库 ---");
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