// application/application.cpp
// 应用层：顶层应用编排器实现
#include "application/application.hpp"

// Application Layer
#include "application/handlers/config_handler.hpp"
#include "application/handlers/workflow_handler.hpp"

// CLI (Presentation Layer)
#include "cli/log_generator_cli.hpp"

// Infrastructure (具体实现)
#include "infrastructure/persistence/file_manager.hpp"
#include "infrastructure/concurrency/thread_pool_executor.hpp"
#include "infrastructure/factories/log_generator_factory.hpp"

// Utils
#include "utils/utils.hpp"
#include "cli/framework/ansi_colors.hpp"

#include <iostream>
#include <memory>

namespace application {

int App::Run(int argc, char* argv[]) {
    try {
        utils::SetupConsole();

        // 1. 解析命令行参数
        cli::LogGeneratorCli cli;
        auto cli_config_opt = cli.Parse(argc, argv);
        
        // 如果解析失败或处理了通用命令（如 --help），直接退出
        if (!cli_config_opt) {
            return 0;
        }

        // 2. 依赖注入组装
        auto file_manager = std::make_shared<infrastructure::persistence::FileManager>();
        auto thread_pool = std::make_shared<infrastructure::concurrency::ThreadPoolExecutor>();
        auto gen_factory = std::make_shared<infrastructure::factories::LogGeneratorFactory>();

        // 3. 配置加载
        handlers::ConfigHandler config_handler(file_manager);
        auto context = config_handler.LoadAndMerge(
            *cli_config_opt, 
            "config/activities_config.toml", 
            "config/mapping_config.toml"
        );

        // 4. 执行工作流
        handlers::WorkflowHandler workflow(file_manager, thread_pool, gen_factory);
        workflow.Run(context);

        return 0;

    } catch (const std::exception& e) {
        std::cerr << RED_COLOR << "\n[Fatal Error] " << e.what() << RESET_COLOR << std::endl;
        return 1;
    } catch (...) {
        std::cerr << RED_COLOR << "\n[Fatal Error] Unknown exception occurred." << RESET_COLOR << std::endl;
        return 1;
    }
}

}  // namespace application
