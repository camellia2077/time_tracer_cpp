// core/workflow/workflow_handler.cpp
#include "core/workflow/workflow_handler.hpp"
#include "utils/utils.hpp"
#include <format>
#include <filesystem>
#include <iostream>
#include <atomic>

namespace Core {

    WorkflowHandler::WorkflowHandler(IFileWriter& file_writer, 
                                     ILogGeneratorFactory& generator_factory,
                                     Common::ITaskExecutor& executor)
        : file_writer_(file_writer), 
          generator_factory_(generator_factory),
          executor_(executor) {}

    int WorkflowHandler::run(const AppContext& context, ReportHandler& report_handler) {
        const std::string master_dir_name = "Date";

        // 1. IO: 准备目录结构 (业务前置条件)
        if (!file_writer_.setup_directories(master_dir_name, context.config.start_year, context.config.end_year)) {
            return -1;
        }

        auto& reporter = report_handler.get_reporter();
        
        // 使用原子变量统计成功生成的文件数，因为这将在多线程中被修改
        // 注意：如果你不想在 lambda 里用 atomic，也可以让 task 返回 future，但这里为了解耦使用了引用捕获
        std::atomic<int> total_files_generated{0};

        std::cout << "Dispatching tasks to executor...\n";

        // 2. 任务分发循环
        for (int year = context.config.start_year; year <= context.config.end_year; ++year) {
            
            // 提交任务到抽象的 Executor
            // 注意：capture 需要确保 context/reporter/master_dir_name 在执行期间有效 (Run 方法栈未销毁)
            executor_.submit([=, this, &context, &reporter, &total_files_generated, &master_dir_name]() {
                
                // --- 业务逻辑开始 (与线程管理无关) ---
                
                // 为该线程/任务创建独立的生成器实例
                auto generator = generator_factory_.create(context);
                
                std::string buffer;
                buffer.reserve(1024 * 1024); // 预分配 1MB

                for (int month = 1; month <= 12; ++month) {
                    // 路径逻辑
                    std::string filename = std::format("{}_{:02}.txt", year, month);
                    std::filesystem::path full_path = std::filesystem::path(master_dir_name) / std::to_string(year) / filename;

                    // 生成阶段
                    auto gen_start = std::chrono::high_resolution_clock::now();
                    generator->generate_for_month(year, month, Utils::get_days_in_month(year, month), buffer);
                    reporter.add_generation_time(std::chrono::high_resolution_clock::now() - gen_start);

                    // 写入阶段
                    auto io_start = std::chrono::high_resolution_clock::now();
                    if (file_writer_.write_log_file(full_path, buffer)) {
                        total_files_generated.fetch_add(1, std::memory_order_relaxed);
                    }
                    reporter.add_io_time(std::chrono::high_resolution_clock::now() - io_start);
                }
                // --- 业务逻辑结束 ---
            });
        }

        // 3. 控制流同步：等待所有任务完成
        // WorkflowHandler 不需要知道是 `std::thread` join 还是 `condition_variable` wait，它只调用 wait_all
        executor_.wait_all();

        return total_files_generated.load();
    }

}