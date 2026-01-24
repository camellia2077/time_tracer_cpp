// core/application/service/workflow_handler.hpp
#ifndef CORE_APPLICATION_SERVICE_WORKFLOW_HANDLER_HPP_
#define CORE_APPLICATION_SERVICE_WORKFLOW_HANDLER_HPP_

#include "core/application/interfaces/i_workflow_handler.hpp"
#include "core/infrastructure/services/import_service.hpp" // 引入新服务
#include <memory>

class WorkflowHandler : public IWorkflowHandler {
public:
    WorkflowHandler(const std::string& db_path, 
                    const AppConfig& config, 
                    const fs::path& output_root_path, 
                    std::shared_ptr<core::interfaces::IFileSystem> fs,
                    std::shared_ptr<core::interfaces::IUserNotifier> notifier);
    ~WorkflowHandler() override; 

    void run_converter(const std::string& input_path, const AppOptions& options) override;
    void run_database_import(const std::string& processed_path_str) override;
    void run_database_import_from_memory(const std::map<std::string, std::vector<DailyLog>>& data_map) override;
    void run_ingest(const std::string& source_path, DateCheckMode date_check_mode, bool save_processed = false) override;
    const AppConfig& get_config() const override;

private:
    const AppConfig& app_config_;
    fs::path output_root_path_;
    std::shared_ptr<core::interfaces::IFileSystem> fs_;
    std::shared_ptr<core::interfaces::IUserNotifier> notifier_;
    
    // 组合 ImportService
    std::unique_ptr<core::service::ImportService> import_service_;
};

#endif