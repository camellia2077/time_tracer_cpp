// core/workflow/workflow_handler.hpp
#ifndef CORE_WORKFLOW_WORKFLOW_HANDLER_HPP_
#define CORE_WORKFLOW_WORKFLOW_HANDLER_HPP_

#include "common/app_context.hpp"
#include "core/reporting/report_handler.hpp"
// [关键] 只包含接口定义
#include "io/file_system_interfaces.hpp" 
#include "generator/api/log_generator_factory.hpp" 
#include "infrastructure/concurrency/i_task_executor.hpp" 

namespace Core {

    class WorkflowHandler {
    public:
        WorkflowHandler(IFileWriter& file_writer, 
                        ILogGeneratorFactory& generator_factory,
                        Common::ITaskExecutor& executor);

        int run(const AppContext& context, ReportHandler& report_handler);

    private:
        IFileWriter& file_writer_;
        ILogGeneratorFactory& generator_factory_;
        Common::ITaskExecutor& executor_;
    };

}

#endif // CORE_WORKFLOW_WORKFLOW_HANDLER_HPP_