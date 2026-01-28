// application/handlers/workflow_handler.hpp
// 应用层：工作流处理器
#pragma once
#ifndef APPLICATION_HANDLERS_WORKFLOW_HANDLER_H_
#define APPLICATION_HANDLERS_WORKFLOW_HANDLER_H_

#include "domain/model/generation_context.hpp"
#include "domain/ports/file_system_interface.hpp"
#include "domain/ports/task_executor_interface.hpp"
#include "application/factories/log_generator_factory_interface.hpp"
#include "utils/performance_reporter.hpp"
#include <memory>

namespace application::handlers {

/**
 * @class WorkflowHandler
 * @brief 日志生成工作流处理器
 * 
 * 职责：
 * 1. 初始化输出目录结构
 * 2. 提交并行生成任务
 * 3. 汇报执行结果
 */
class WorkflowHandler {
public:
    WorkflowHandler(
        std::shared_ptr<domain::ports::FileWriterInterface> file_writer,
        std::shared_ptr<domain::ports::TaskExecutorInterface> task_executor,
        std::shared_ptr<application::factories::LogGeneratorFactoryInterface> generator_factory
    );

    void Run(const domain::model::GenerationContext& context);

private:
    void ExecuteMonthTask(
        const domain::model::GenerationContext& context,
        int year,
        int month,
        utils::PerformanceReporter& reporter
    );

    std::shared_ptr<domain::ports::FileWriterInterface> file_writer_;
    std::shared_ptr<domain::ports::TaskExecutorInterface> task_executor_;
    std::shared_ptr<application::factories::LogGeneratorFactoryInterface> generator_factory_;
};

}  // namespace application::handlers

#endif  // APPLICATION_HANDLERS_WORKFLOW_HANDLER_H_
