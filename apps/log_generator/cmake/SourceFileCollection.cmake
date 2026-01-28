
# 负责管理源文件列表
# --- Source Files Definition ---

set(SOURCES
    # 入口
    src/main.cpp

    # Application Layer (应用层)
    src/application/application.cpp
    src/application/handlers/config_handler.cpp
    src/application/handlers/workflow_handler.cpp

    # Infrastructure Layer (基础设施层)
    src/infrastructure/concurrency/thread_pool_executor.cpp
    src/infrastructure/persistence/file_manager.cpp
    src/infrastructure/persistence/toml_config_parser.cpp

    # Generator Module (生成器模块)
    src/generator/log_generator.cpp
    src/generator/components/remark_generator.cpp
    src/generator/components/event_generator.cpp
    src/generator/components/day_generator.cpp
    src/generator/strategies/sleep_scheduler.cpp

    # CLI Layer (命令行界面层)
    src/cli/log_generator_cli.cpp
    src/cli/framework/arg_parser.cpp
    src/cli/framework/help_formatter.cpp

    # Utils (工具模块)
    src/utils/performance_reporter.cpp
    src/utils/utils.cpp
)