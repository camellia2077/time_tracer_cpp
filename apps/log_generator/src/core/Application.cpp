// core/application.cpp
#include "core/application.hpp"
#include "core/config/config_handler.hpp"
#include "core/workflow/workflow_handler.hpp"
#include "core/reporting/report_handler.hpp"
#include "utils/utils.hpp"

// [新增] 引入 CLI 解析器
#include "cli/impl/log_generator_cli.hpp" 

// [实现类]
#include "io/file_manager.hpp"
#include "generator/api/log_generator_factory.hpp" 
#include "infrastructure/concurrency/thread_pool_executor.hpp" 

namespace Core {

    int Application::run(int argc, char* argv[]) {
        Utils::setup_console();

        // 1. [CLI Layer] 解析命令行参数
        // Application 现在负责这一步，而不是委托给 ConfigHandler
        Cli::Impl::LogGeneratorCli cli;
        auto config_opt = cli.parse(argc, argv);

        if (!config_opt) {
            return 0; // 打印了帮助/版本或参数错误，直接退出
        }

        // 2. [实例化依赖]
        FileManager file_manager;
        ConfigHandler config_handler;

        // 3. [加载业务配置]
        // 传入 CLI 解析出的 config 和 exe 路径
        std::filesystem::path exe_path(argv[0]);
        auto context_opt = config_handler.load(*config_opt, exe_path, file_manager);
        
        if (!context_opt) return 1; // 加载失败

        // 4. [执行工作流]
        ReportHandler report_handler;
        LogGeneratorFactory generator_factory;
        Common::ThreadPoolExecutor executor;
        
        WorkflowHandler workflow(file_manager, generator_factory, executor);
        
        int files_generated = workflow.run(*context_opt, report_handler);

        if (files_generated < 0) return 1; 

        // 5. [报告]
        report_handler.finish(context_opt->config, files_generated);

        return 0;
    }

}