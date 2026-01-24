// core/workflow/workflow_handler.hpp
#pragma once
#ifndef CORE_WORKFLOW_WORKFLOW_HANDLER_HPP_
#define CORE_WORKFLOW_WORKFLOW_HANDLER_HPP_

#include "common/app_context.hpp"
#include "generator/api/log_generator_factory.hpp"
#include "infrastructure/concurrency/i_task_executor.hpp"
#include "io/file_system_interfaces.hpp"
#include "utils/performance_reporter.hpp"
#include <memory>

namespace Core::Workflow {

    class WorkflowHandler {
    public:
        WorkflowHandler(std::shared_ptr<IFileWriter> file_writer,
                        std::shared_ptr<Common::ITaskExecutor> task_executor,
                        std::shared_ptr<ILogGeneratorFactory> generator_factory);

        void run(const AppContext& context);

    private:
        // 处理单月生成的任务逻辑
        void execute_month_task(const AppContext& context, 
                                int year, 
                                int month, 
                                PerformanceReporter& reporter);

        std::shared_ptr<IFileWriter> file_writer_;
        std::shared_ptr<Common::ITaskExecutor> task_executor_;
        std::shared_ptr<ILogGeneratorFactory> generator_factory_;
    };

}

#endif // CORE_WORKFLOW_WORKFLOW_HANDLER_HPP_