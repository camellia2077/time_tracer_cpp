// application/service/workflow_handler.hpp
#ifndef APPLICATION_SERVICE_WORKFLOW_HANDLER_HPP_
#define APPLICATION_SERVICE_WORKFLOW_HANDLER_HPP_

#include "application/interfaces/i_log_converter.hpp" // [必须包含]
#include "application/interfaces/i_log_serializer.hpp"
#include "application/interfaces/i_workflow_handler.hpp"
#include "core/infrastructure/services/import_service.hpp"
#include <memory>

class WorkflowHandler : public IWorkflowHandler {
public:
  // [修改] 构造函数接�?7 个参�?(新增 serializer �?converter)
  WorkflowHandler(const std::string &db_path, const AppConfig &config,
                  const fs::path &output_root_path,
                  std::shared_ptr<core::interfaces::IFileSystem> fs,
                  std::shared_ptr<core::interfaces::IUserNotifier> notifier,
                  std::shared_ptr<core::interfaces::ILogSerializer> serializer,
                  std::shared_ptr<core::interfaces::ILogConverter> converter);

  ~WorkflowHandler() override;

  void RunConverter(const std::string &input_path,
                    const AppOptions &options) override;
  void RunDatabaseImport(const std::string &processed_path_str) override;
  void RunDatabaseImportFromMemory(
      const std::map<std::string, std::vector<DailyLog>> &data_map) override;
  void RunIngest(const std::string &source_path, DateCheckMode date_check_mode,
                 bool save_processed = false) override;
  const AppConfig &GetConfig() const override;

private:
  const AppConfig &app_config_;
  fs::path output_root_path_;
  std::shared_ptr<core::interfaces::IFileSystem> fs_;
  std::shared_ptr<core::interfaces::IUserNotifier> notifier_;

  // [新增] 持有注入的依�?
  std::shared_ptr<core::interfaces::ILogSerializer> serializer_;
  std::shared_ptr<core::interfaces::ILogConverter> converter_;

  std::unique_ptr<core::service::ImportService> import_service_;
};

#endif
