// cli/impl/app/app_context.hpp
#ifndef CLI_IMPL_APP_APP_CONTEXT_HPP_
#define CLI_IMPL_APP_APP_CONTEXT_HPP_

#include <memory>
#include "common/config/app_config.hpp" 

class IWorkflowHandler;
class IReportHandler;
namespace core::interfaces { 
    class IFileSystem; 
    class IUserNotifier; // [新增]
}

struct AppContext {
    std::shared_ptr<IWorkflowHandler> workflow_handler;
    std::shared_ptr<IReportHandler> report_handler;
    
    std::shared_ptr<core::interfaces::IFileSystem> file_system;
    std::shared_ptr<core::interfaces::IUserNotifier> user_notifier; // [新增]
    
    AppConfig config; 
};

#endif // CLI_IMPL_APP_APP_CONTEXT_HPP_