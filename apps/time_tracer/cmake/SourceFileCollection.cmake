# src/cmake/SourceFileCollection.cmake
# 模块1：源文件收集
# 职责：仅定义源文件列表变量，不创建任何构建目标。

# ----------------------------------------------------
# 显式列出所有源文件以确保构建系统的稳定性和可预测性。
# --- Application Sources ---
set(APPLICATION_SOURCES
    # Application - Root
    src/application/pipeline_factory.cpp
    src/application/pipeline_runner.cpp

    # Application - Handlers (NEW - Command/Handler Pattern)
    src/application/handlers/ingest_handler.cpp
    src/application/handlers/export_handler.cpp

    # Application - Service
    src/application/service/report_generator.cpp
    src/application/service/report_handler.cpp
    src/application/service/workflow_handler.cpp

    # Application - Steps
    src/application/steps/config_loader_step.cpp
    src/application/steps/converter_step.cpp
    src/application/steps/file_collector.cpp
    src/application/steps/logic_linker_step.cpp
    src/application/steps/logic_validator_step.cpp
    src/application/steps/processed_data_writer_step.cpp
    src/application/steps/structure_validator_step.cpp

    # Application - Utils
    src/application/utils/converter_config_factory.cpp
    src/application/utils/processed_data_writer.cpp
)

# --- Core Sources (Infrastructure & Domain) ---
set(CORE_SOURCES
    # Infrastructure - Persistence
    src/core/infrastructure/persistence/db_manager.cpp
    src/core/infrastructure/persistence/sqlite_report_repository_adapter.cpp

    # Infrastructure - Reporting
    src/core/infrastructure/reporting/export_utils.cpp
    src/core/infrastructure/reporting/exporter.cpp
    src/core/infrastructure/reporting/report_file_manager.cpp

    # Infrastructure - Services
    src/core/infrastructure/services/import_service.cpp
)

# --- Common  ---
set(COMMON_SOURCES
    "src/common/utils/time_utils.cpp"
)
set(SERIALIZER_SOURCES
    "src/serializer/json_serializer.cpp"
    "src/serializer/core/log_deserializer.cpp"
    "src/serializer/core/log_serializer.cpp"
)

set(VALIDATOR_SOURCES

    # common 模块
    "src/validator/common/validator_utils.cpp"

    # logic 模块
    "src/validator/logic/facade/logic_validator.cpp"
    "src/validator/logic/rules/date_rules.cpp"
    "src/validator/logic/rules/activity_rules.cpp"
    


    # source_txt 模块
    "src/validator/txt/facade/text_validator.cpp"
    "src/validator/txt/rules/line_rules.cpp"
    "src/validator/txt/rules/structure_rules.cpp"
)


# --- Shared Reports Library ---
# 创建一个静态库，包含所有报告生成器共享的源文件
# 这样可以避免在主程序和多个DLL中重复编译相同的代码

set(REPORTS_SHARED_SOURCES
    # 外部依赖或通用工具
    "src/common/utils/string_utils.cpp"
    
    # 基础配置
    "src/reports/daily/common/day_base_config.cpp"
    # [新增] Range 基础配置 (替代 Month/Period)
    "src/reports/range/common/range_base_config.cpp"
    # [删除] Month/Period Base Config (已移除)

    "src/reports/daily/formatters/statistics/stat_formatter.cpp"

    # Shared - 内部工具与配置
    "src/reports/core/utils/report_string_utils.cpp"
    "src/reports/core/utils/report_time_format.cpp"
    
    # Shared - 样式配置
    "src/reports/core/config/export_style_config.cpp"

    # Shared - 格式化器实现
    "src/reports/core/formatters/base/project_tree_formatter.cpp"
    "src/reports/core/formatters/markdown/markdown_formatter.cpp" 
    "src/reports/core/formatters/typst/typ_utils.cpp"
    "src/reports/core/formatters/latex/tex_utils.cpp"
)

# ==========================================
# REPORTS_DATA_SOURCES (数据获取、查询逻辑、树构建)
# ==========================================
set(REPORTS_DATA_SOURCES
    # 工具类
    "src/reports/data/utils/project_tree_builder.cpp"

    "src/reports/data/repositories/sqlite_report_data_repository.cpp"

    # Daily Queriers (保留)
    "src/reports/data/queriers/daily/day_querier.cpp"
    "src/reports/data/queriers/daily/batch_day_data_fetcher.cpp"
)

set(REPORTS_SOURCES
    # [新增] 统一的 Range Service (替代 Monthly/Period Service)
    "src/reports/services/daily_report_service.cpp"
    "src/reports/services/range_report_service.cpp"
)


# --- Time Master CLI Sources ---
set(CLI_SOURCES
    # --- Framework (合并后的文件) ---
    "src/cli/framework/command.cpp"
    "src/cli/framework/console_io.cpp"

    # --- Impl ---
    "src/cli/impl/app/cli_application.cpp"
    "src/cli/impl/utils/help_formatter.cpp"
    "src/cli/impl/commands/all_commands.cpp"
)
# --- DB Inserter Sources ---
set(BOOTSTRAP_SOURCES
    "src/bootstrap/startup_validator.cpp"
)

# --- Importer Sources (Database Ingestion Module) ---
set(IMPORTER_SOURCES
    # Top-level Facade & Service
    "src/importer/data_importer.cpp"
    "src/importer/import_service.cpp"

    # Parser Layer (Parsing logic)
    "src/importer/parser/memory_parser.cpp"

    # Storage Layer (Database persistence)
    "src/importer/storage/repository.cpp"
    "src/importer/storage/sqlite/writer.cpp"
    "src/importer/storage/sqlite/project_resolver.cpp"
    "src/importer/storage/sqlite/connection.cpp"
    "src/importer/storage/sqlite/statement.cpp"
)



# --- converter Sources ---
set(CONVERTER_SOURCES
    "src/converter/log_processor.cpp"
    # --- 转换模块 (Convert) ---
    "src/converter/convert/facade/converter_service.cpp"
    "src/converter/convert/core/log_linker.cpp"
    "src/converter/convert/core/activity_mapper.cpp"
    "src/converter/convert/core/day_processor.cpp"
    "src/converter/convert/core/day_stats.cpp"
    "src/converter/convert/io/text_parser.cpp"
)

set(IO_SOURCES
    "src/io/disk_file_system.cpp"
)

set(CONFIG_SOURCES
    "src/config/config_loader.cpp"

    # 合并后的解析工具
    "src/config/parser_utils.cpp"

    # Loaders (合并后)
    "src/config/loaders/converter_loader.cpp"
    "src/config/loaders/report_loader.cpp"

    # Validators (合并后)
    "src/config/validators/validator_facade.cpp"
    "src/config/validators/converter_validator.cpp"
    "src/config/validators/report_validator.cpp"
    "src/config/validators/plugin_validator.cpp"
)




message(STATUS "Source files collected explicitly.")