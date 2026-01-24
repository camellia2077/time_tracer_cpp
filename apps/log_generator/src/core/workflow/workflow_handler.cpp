// core/workflow/workflow_handler.cpp
#include "core/workflow/workflow_handler.hpp"
#include "utils/utils.hpp"
#include "common/ansi_colors.hpp"
#include <iostream>
#include <chrono>
#include <format>
#include <atomic>

namespace Core::Workflow {

    WorkflowHandler::WorkflowHandler(std::shared_ptr<IFileWriter> file_writer,
                                     std::shared_ptr<Common::ITaskExecutor> task_executor,
                                     std::shared_ptr<ILogGeneratorFactory> generator_factory)
        : file_writer_(std::move(file_writer)),
          task_executor_(std::move(task_executor)),
          generator_factory_(std::move(generator_factory)) {}

    void WorkflowHandler::run(const AppContext& context) {
        auto start_time = std::chrono::high_resolution_clock::now();
        PerformanceReporter reporter;
        std::atomic<int> files_generated{0};

        // 1. 初始化目录结构
        std::string master_dir = "logs"; // 硬编码或从配置读取
        if (!file_writer_->setup_directories(master_dir, context.config.start_year, context.config.end_year)) {
            return;
        }

        std::cout << "Starting generation for years " << context.config.start_year 
                  << " to " << context.config.end_year << "...\n";

        // 2. 提交任务
        for (int year = context.config.start_year; year <= context.config.end_year; ++year) {
            for (int month = 1; month <= 12; ++month) {
                // Lambda 捕获 context 引用是危险的如果 context 生命周期短于任务
                // 但在此架构中 AppContext 在 main 中存活，且 wait_all 保证同步，故安全。
                // 为了保险，按值捕获基础类型，引用捕获大对象
                task_executor_->submit([this, &context, &reporter, &files_generated, year, month, master_dir]() {
                    execute_month_task(context, year, month, reporter);
                    files_generated.fetch_add(1, std::memory_order_relaxed);
                });
            }
        }

        // 3. 等待所有任务完成
        task_executor_->wait_all();

        // 4. 汇报
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = end_time - start_time;
        reporter.report(context.config, files_generated.load(), duration);
    }

    void WorkflowHandler::execute_month_task(const AppContext& context, 
                                             int year, 
                                             int month, 
                                             PerformanceReporter& reporter) {
        // [线程安全关键点] 
        // LogGenerator 包含随机数引擎，不可跨线程共享。
        // 必须在任务内部通过工厂创建一个新的实例。
        auto generator = generator_factory_->create(context);

        // 1. 生成逻辑
        auto t1 = std::chrono::high_resolution_clock::now();
        
        std::string buffer;
        int days = Utils::get_days_in_month(year, month);
        generator->generate_for_month(year, month, days, buffer);
        
        auto t2 = std::chrono::high_resolution_clock::now();
        reporter.add_generation_time(t2 - t1);

        // 2. IO 逻辑
        auto t3 = std::chrono::high_resolution_clock::now();
        
        std::filesystem::path path = std::filesystem::path("logs") 
                                     / std::to_string(year) 
                                     / std::format("{:02}.txt", month); // 假设文件名格式
        file_writer_->write_log_file(path, buffer);
        
        auto t4 = std::chrono::high_resolution_clock::now();
        reporter.add_io_time(t4 - t3);
    }

}