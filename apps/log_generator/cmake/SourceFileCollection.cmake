
# 负责管理源文件列表
# --- Source Files Definition ---

set(SOURCES
    # 入口
    src/main.cpp
    src/infrastructure/concurrency/thread_pool_executor.cpp

    # Core
    src/core/config/config_handler.cpp
    src/core/application.cpp
    src/core/workflow/workflow_handler.cpp
    
    # CLI 命令行模块
        
    src/cli/framework/arg_parser.cpp
    src/cli/framework/help_formatter.cpp
    src/cli/impl/log_generator_cli.cpp
    src/cli/impl/config_validator.cpp
    src/cli/impl/config_builder.cpp
    
    # Config 配置模块 (旧有的类)
    src/config/config.cpp
    
    # FileIO 文件模块
    src/io/file_manager.cpp
    
    # Generator 生成器模块
    src/generator/components/remark_generator.cpp
    src/generator/components/event_generator.cpp
    src/generator/components/day_generator.cpp

    # Impl (核心实现)
    src/generator/impl/log_generator.cpp

    # Strategies (策略/算法)
    src/generator/strategies/sleep_scheduler.cpp
    
    # Utils 工具模块
    src/utils/performance_reporter.cpp
    src/utils/utils.cpp
)