// core/application.cpp
#include "core/application.hpp"

// Layer Includes
#include "cli/impl/log_generator_cli.hpp"
#include "core/config/config_handler.hpp"
#include "core/workflow/workflow_handler.hpp"

// Infrastructure & Implementations
#include "io/file_manager.hpp"
#include "infrastructure/concurrency/thread_pool_executor.hpp"
#include "generator/api/log_generator_factory.hpp"
#include "utils/utils.hpp"
#include "common/ansi_colors.hpp"

#include <iostream>
#include <memory>

namespace Core {

    int Application::run(int argc, char* argv[]) {
        try {
            Utils::setup_console();

            // 1. 解析命令行参数
            Cli::Impl::LogGeneratorCli cli;
            auto cli_config_opt = cli.parse(argc, argv);
            
            // 如果解析失败或处理了通用命令（如 --help），直接退出
            if (!cli_config_opt) {
                return 0; // 或者根据具体情况返回 1
            }

            // 2. 依赖注入组装
            auto file_manager = std::make_shared<FileManager>();
            auto thread_pool = std::make_shared<Common::ThreadPoolExecutor>(); // 默认使用硬件并发数
            auto gen_factory = std::make_shared<LogGeneratorFactory>();

            // 3. 配置加载
            Config::ConfigHandler config_handler(file_manager);
            // 配置文件位于当前工作目录的 config 子目录下
            AppContext context = config_handler.load_and_merge(
                *cli_config_opt, 
                "config/activities_config.toml", 
                "config/mapping_config.toml"
            );

            // 4. 执行工作流
            Workflow::WorkflowHandler workflow(file_manager, thread_pool, gen_factory);
            workflow.run(context);

            return 0;

        } catch (const std::exception& e) {
            std::cerr << RED_COLOR << "\n[Fatal Error] " << e.what() << RESET_COLOR << std::endl;
            return 1;
        } catch (...) {
            std::cerr << RED_COLOR << "\n[Fatal Error] Unknown exception occurred." << RESET_COLOR << std::endl;
            return 1;
        }
    }

}