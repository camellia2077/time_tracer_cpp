// cli/impl/app/app_context.hpp
#ifndef CLI_IMPL_APP_APP_CONTEXT_HPP_
#define CLI_IMPL_APP_APP_CONTEXT_HPP_

#include <memory>
#include "common/config/app_config.hpp" 

class IWorkflowHandler;
class IReportHandler;
namespace core::interfaces { 
    class IFileSystem; 
    class IUserNotifier;
    class ILogSerializer;
    class ILogConverter; // [新增] 前向声明 Converter 接口
}

struct AppContext {
    std::shared_ptr<IWorkflowHandler> workflow_handler;
    std::shared_ptr<IReportHandler> report_handler;
    
    std::shared_ptr<core::interfaces::IFileSystem> file_system;
    std::shared_ptr<core::interfaces::IUserNotifier> user_notifier;
    
    std::shared_ptr<core::interfaces::ILogSerializer> serializer;
    std::shared_ptr<core::interfaces::ILogConverter> log_converter; // [新增] 存放 Converter 实例
    
    AppConfig config; 
};

#endif // CLI_IMPL_APP_APP_CONTEXT_HPP_