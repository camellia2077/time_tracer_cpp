// application/handlers/workflow_handler.cpp
// 应用层：工作流处理器实现
#include "application/handlers/workflow_handler.hpp"
#include "utils/utils.hpp"
#include "cli/framework/ansi_colors.hpp"
#include <iostream>
#include <chrono>
#include <format>
#include <atomic>

namespace application::handlers {

WorkflowHandler::WorkflowHandler(
    std::shared_ptr<domain::ports::FileWriterInterface> file_writer,
    std::shared_ptr<domain::ports::TaskExecutorInterface> task_executor,
    std::shared_ptr<application::factories::LogGeneratorFactoryInterface> generator_factory)
    : file_writer_(std::move(file_writer)),
      task_executor_(std::move(task_executor)),
      generator_factory_(std::move(generator_factory)) {}

void WorkflowHandler::Run(const domain::model::GenerationContext& context) {
    auto start_time = std::chrono::high_resolution_clock::now();
    utils::PerformanceReporter reporter;
    std::atomic<int> files_generated{0};

    // 1. 初始化目录结构
    std::string master_dir = "logs";
    if (!file_writer_->SetupDirectories(master_dir, context.config.start_year, context.config.end_year)) {
        return;
    }

    std::cout << "Starting generation for years " << context.config.start_year 
              << " to " << context.config.end_year << "...\n";

    // 2. 提交任务
    for (int year = context.config.start_year; year <= context.config.end_year; ++year) {
        for (int month = 1; month <= 12; ++month) {
            task_executor_->Submit([this, &context, &reporter, &files_generated, year, month]() {
                ExecuteMonthTask(context, year, month, reporter);
                files_generated.fetch_add(1, std::memory_order_relaxed);
            });
        }
    }

    // 3. 等待所有任务完成
    task_executor_->WaitAll();

    // 4. 汇报
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = end_time - start_time;
    reporter.Report(context.config, files_generated.load(), duration);
}

void WorkflowHandler::ExecuteMonthTask(
    const domain::model::GenerationContext& context,
    int year,
    int month,
    utils::PerformanceReporter& reporter) {
    
    // LogGenerator 包含随机数引擎，不可跨线程共享
    // 必须在任务内部通过工厂创建一个新的实例
    auto generator = generator_factory_->Create(context);

    // 1. 生成逻辑
    auto t1 = std::chrono::high_resolution_clock::now();
    
    std::string buffer;
    int days = utils::GetDaysInMonth(year, month);
    generator->GenerateForMonth(year, month, days, buffer);
    
    auto t2 = std::chrono::high_resolution_clock::now();
    reporter.AddGenerationTime(t2 - t1);

    // 2. IO 逻辑
    auto t3 = std::chrono::high_resolution_clock::now();
    
    std::filesystem::path path = std::filesystem::path("logs") 
                                 / std::to_string(year) 
                                 / std::format("{}-{:02}.txt", year, month);
    file_writer_->WriteLogFile(path, buffer);
    
    auto t4 = std::chrono::high_resolution_clock::now();
    reporter.AddIoTime(t4 - t3);
}

}  // namespace application::handlers
