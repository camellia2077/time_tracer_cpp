# src/cmake/SourceFileCollection.cmake
# 模块1：源文件收集
# 职责：仅定义源文件列表变量，不创建任何构建目标。

# ----------------------------------------------------
# 显式列出所有源文件以确保构建系统的稳定性和可预测性。
# --- Core Sources ---
set(CORE_SOURCES
    # Application - Root
    src/core/application/pipeline_factory.cpp
    src/core/application/pipeline_runner.cpp

    # Application - Service
    src/core/application/service/report_generator.cpp
    src/core/application/service/report_handler.cpp
    src/core/application/service/workflow_handler.cpp

    # Application - Steps
    src/core/application/steps/config_loader_step.cpp
    src/core/application/steps/converter_step.cpp
    src/core/application/steps/file_collector.cpp
    src/core/application/steps/logic_linker_step.cpp
    src/core/application/steps/logic_validator_step.cpp
    src/core/application/steps/processed_data_writer_step.cpp
    src/core/application/steps/structure_validator_step.cpp

    # Application - Utils
    src/core/application/utils/converter_config_factory.cpp
    src/core/application/utils/processed_data_writer.cpp

    # Infrastructure - Persistence
    src/core/infrastructure/persistence/db_manager.cpp
    src/core/infrastructure/persistence/sqlite_report_repository.cpp

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
    
    # 基础配置 (原有的)
    "src/reports/daily/common/day_base_config.cpp"
    "src/reports/monthly/common/month_base_config.cpp"
    "src/reports/period/common/period_base_config.cpp"
    "src/reports/daily/formatters/statistics/stat_formatter.cpp"

    # Shared - 内部工具与配置
    "src/reports/shared/utils/format/bool_to_string.cpp"
    "src/reports/shared/utils/format/report_string_utils.cpp"
    "src/reports/shared/utils/format/time_format.cpp"
    
    # Shared - 样式配置
    "src/reports/shared/config/tex_style_config.cpp"
    "src/reports/shared/config/typst_style_config.cpp"

    # Shared - 格式化器实现
    "src/reports/shared/formatters/base/project_tree_formatter.cpp"
    "src/reports/shared/formatters/markdown/markdown_formatter.cpp" 
    "src/reports/shared/formatters/typst/typ_utils.cpp"
    "src/reports/shared/formatters/latex/tex_utils.cpp"
    "src/reports/shared/formatters/latex/tex_common_utils.cpp"
)

# ==========================================
# REPORTS_DATA_SOURCES (数据获取、查询逻辑、树构建)
# ==========================================
set(REPORTS_DATA_SOURCES
    # 工具类 (根据目录结构 project_tree_builder 在这里)
    "src/reports/data/utils/project_tree_builder.cpp"

    # Daily Queriers
    "src/reports/data/queriers/daily/day_querier.cpp"
    "src/reports/data/queriers/daily/batch_day_data_fetcher.cpp"

    # Monthly Queriers
    "src/reports/data/queriers/monthly/month_querier.cpp"
    "src/reports/data/queriers/monthly/batch_month_data_fetcher.cpp"

    # Period Queriers
    "src/reports/data/queriers/period/period_querier.cpp"
    "src/reports/data/queriers/period/batch_period_data_fetcher.cpp"
)

set(REPORTS_SOURCES
    # Root
    "src/reports/report_service.cpp"
    # Export
    "src/reports/services/daily_report_service.cpp"
    "src/reports/services/monthly_report_service.cpp"
    "src/reports/services/period_report_service.cpp"
)


# --- Time Master CLI Sources ---
set(CLI_SOURCES
    # --- Framework (Core Logic) ---
    "src/cli/framework/core/command_parser.cpp"
    "src/cli/framework/core/arg_definitions.cpp"
    "src/cli/framework/io/console_input.cpp"

    "src/cli/impl/app/cli_application.cpp"
    "src/cli/impl/utils/help_formatter.cpp"
    "src/cli/impl/ui/console_notifier.cpp"

    "src/cli/impl/commands/export/export_command.cpp"

    "src/cli/impl/commands/query/query_command.cpp"

    "src/cli/impl/commands/pipeline/convert_command.cpp"
    "src/cli/impl/commands/pipeline/import_command.cpp"
    "src/cli/impl/commands/pipeline/ingest_command.cpp"
    "src/cli/impl/commands/pipeline/validate_logic_command.cpp"
    "src/cli/impl/commands/pipeline/validate_structure_command.cpp"
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

    "src/config/internal/config_parser_utils.cpp"
    "src/config/loader/report_config_loader.cpp"
    "src/config/loader/converter_config_loader.cpp"
    "src/config/loader/toml_converter_config_loader.cpp"
    
    "src/config/loader/toml_loader_utils.cpp"


    # 顶层外观
    "src/config/validator/facade/config_facade.cpp"

    # converter 领域的 Facade 和 Pipelines
    "src/config/validator/converter/facade/converter_facade.cpp"
    "src/config/validator/converter/rules/duration_rule.cpp"
    "src/config/validator/converter/rules/main_rule.cpp"
    "src/config/validator/converter/rules/mapping_rule.cpp"

    "src/config/validator/plugins/facade/plugin_validator.cpp"

    # reports 领域的 Facade
    "src/config/validator/reports/facade/query_facade.cpp"

    # reports 领域的 Strategies
    "src/config/validator/reports/strategies/base_strategy.cpp"
    "src/config/validator/reports/strategies/strategy_factory.cpp"

    # reports 领域的 Strategies
    "src/config/validator/reports/strategies/daily/daily_md.cpp"
    "src/config/validator/reports/strategies/daily/daily_tex.cpp"
    "src/config/validator/reports/strategies/daily/daily_typ.cpp"
    "src/config/validator/reports/strategies/monthly/monthly.cpp"
    "src/config/validator/reports/strategies/periodic/periodic.cpp"
)




message(STATUS "Source files collected explicitly.")