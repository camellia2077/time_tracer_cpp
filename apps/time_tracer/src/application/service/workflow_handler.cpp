// application/service/workflow_handler.cpp
#include "application/service/workflow_handler.hpp"
#include "application/pipeline/factory.hpp"
#include "core/infrastructure/persistence/db_manager.hpp"
// [移除] #include "serializer/json_serializer.hpp"  <-- 不需要了，只依赖接口
// [移除] #include "converter/log_processor.hpp"     <-- 不需要了，只依赖接口
#include <stdexcept>

// [修改] 构造函数实现：接收并保存所有依�?
WorkflowHandler::WorkflowHandler(
    const std::string &db_path, const AppConfig &config,
    const fs::path &output_root_path,
    std::shared_ptr<core::interfaces::IFileSystem> fs,
    std::shared_ptr<core::interfaces::IUserNotifier> notifier,
    std::shared_ptr<core::interfaces::ILogSerializer> serializer,
    std::shared_ptr<core::interfaces::ILogConverter> converter)
    : app_config_(config), output_root_path_(output_root_path), fs_(fs),
      notifier_(notifier), serializer_(serializer), // 保存
      converter_(converter)                         // 保存
{
  // [修改] 创建 ImportService 时，传递已经注入进来的 serializer_
  import_service_ = std::make_unique<core::service::ImportService>(
      db_path, fs, notifier, serializer_);
}

WorkflowHandler::~WorkflowHandler() = default;

void WorkflowHandler::RunConverter(const std::string &input_path,
                                   const AppOptions &options) {
  core::pipeline::PipelineContext context(app_config_, output_root_path_, fs_,
                                          notifier_);
  context.config.input_root_ = input_path;
  context.config.date_check_mode_ = options.date_check_mode_;
  context.config.save_processed_output_ = options.save_processed_output_;

  // [修改] 将保存的 serializer_ ?converter_ 传递给 PipelineFactory
  auto pipeline = core::pipeline::PipelineFactory::CreateIngestPipeline(
      options, app_config_, serializer_, converter_);

  pipeline->Run(std::move(context));
}

const AppConfig &WorkflowHandler::GetConfig() const { return app_config_; }

void WorkflowHandler::RunDatabaseImport(const std::string &processed_path_str) {
  import_service_->ImportFromFiles(processed_path_str);
}

void WorkflowHandler::RunDatabaseImportFromMemory(
    const std::map<std::string, std::vector<DailyLog>> &data_map) {
  import_service_->ImportFromMemory(data_map);
}

void WorkflowHandler::RunIngest(const std::string &source_path,
                                DateCheckMode date_check_mode,
                                bool save_processed) {
  notifier_->NotifyInfo("\n--- 启动数据摄入 (Ingest) ---");

  AppOptions full_options;
  full_options.validate_structure_ = true;
  full_options.convert_ = true;
  full_options.validate_logic_ = true;
  full_options.date_check_mode_ = date_check_mode;
  full_options.save_processed_output_ = save_processed;

  core::pipeline::PipelineContext context(app_config_, output_root_path_, fs_,
                                          notifier_);
  context.config.input_root_ = source_path;
  context.config.date_check_mode_ = date_check_mode;
  context.config.save_processed_output_ = save_processed;

  // [修改] 传递依赖给 PipelineFactory
  auto pipeline = core::pipeline::PipelineFactory::CreateIngestPipeline(
      full_options, app_config_, serializer_, converter_);

  auto result_context_opt = pipeline->Run(std::move(context));

  if (result_context_opt) {
    const auto &result = result_context_opt->result;
    if (!result.processed_data.empty()) {
      notifier_->NotifyInfo("\n--- 流水线验证通过，准备入库 ---");
      import_service_->ImportFromMemory(result.processed_data);
      notifier_->NotifySuccess("\n=== Ingest 执行成功 ===");
    } else {
      notifier_->NotifyWarning("\n=== Ingest 完成但无数据产生 ===");
    }
  } else {
    notifier_->NotifyError("\n=== Ingest 执行失败 ===");
    throw std::runtime_error("Ingestion process failed.");
  }
}
