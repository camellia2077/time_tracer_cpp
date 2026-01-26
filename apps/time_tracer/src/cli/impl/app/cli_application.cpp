// cli/impl/app/cli_application.cpp
#include "cli/impl/app/cli_application.hpp"
#include "cli/impl/utils/help_formatter.hpp"
#include "io/disk_file_system.hpp"
#include "cli/impl/ui/console_notifier.hpp" 
#include "core/infrastructure/persistence/sqlite_report_repository.hpp"

// Implementation Includes
#include "serializer/json_serializer.hpp"
#include "converter/log_processor.hpp" // [新增] 仅在此处引入具体实现类
#include "core/application/service/workflow_handler.hpp"
#include "core/application/service/report_handler.hpp"
#include "core/infrastructure/persistence/db_manager.hpp"
#include "core/infrastructure/reporting/exporter.hpp"
#include "core/application/service/report_generator.hpp"
#include "cli/framework/core/command_registry.hpp"
#include "config/config_loader.hpp"

namespace fs = std::filesystem;
const std::string DATABASE_FILENAME = "time_data.sqlite3";

static ParserConfig get_app_parser_config() {
    return {
        { "-o", "--output", "-f", "--format", "--date-check", "--db", "--database" },
        { "--save-processed", "--no-save", "--no-date-check" }
    };
}

CliApplication::CliApplication(const std::vector<std::string>& args)
    : parser_(args, get_app_parser_config()) {
    
    // 0. 初始化服务容器
    app_context_ = std::make_shared<AppContext>();

    // 1. 路径初始化
    fs::path exe_path(parser_.get_raw_arg(0));
    fs::path default_output_root = fs::absolute(exe_path.parent_path() / "output");

    fs::path db_path;
    if (auto user_db_path = parser_.get_option({"--db", "--database"})) {
        db_path = fs::absolute(*user_db_path);
    } else {
        db_path = default_output_root / DATABASE_FILENAME;
    }

    if (auto path_opt = parser_.get_option({"-o", "--output"})) {
        exported_files_path_ = fs::absolute(*path_opt);
    } else {
        exported_files_path_ = default_output_root / "exported_files";
    }
    output_root_path_ = default_output_root;

    // 2. 基础设施初始化
    auto disk_fs = std::make_shared<io::DiskFileSystem>();
    app_context_->file_system = disk_fs;

    auto notifier = std::make_shared<ConsoleNotifier>();
    app_context_->user_notifier = notifier;

    // 3. 插件初始化 (Serializer & Converter)
    auto serializer = std::make_shared<serializer::JsonSerializer>();
    app_context_->serializer = serializer;

    // [新增] 实例化具体的 Converter 并注入 Context
    // LogProcessor 现在的构造函数是无参的（无状态），配置在运行时传入
    auto converter = std::make_shared<LogProcessor>(); 
    app_context_->log_converter = converter;

    // 4. 加载配置
    ConfigLoader config_loader(parser_.get_raw_arg(0), disk_fs);
    app_config_ = config_loader.load_configuration(); 
    app_context_->config = app_config_;

    // 5. 初始化目录
    try {
        disk_fs->create_directories(output_root_path_);
        disk_fs->create_directories(exported_files_path_);
    } catch (const std::exception& e) {
        notifier->notify_error("Fatal Error: Failed to initialize output directories: " + std::string(e.what()));
        throw; 
    }
    
    // 6. 初始化数据库管理器
    db_manager_ = std::make_unique<DBManager>(db_path.string());

    // 7. 初始化 Core Service (WorkflowHandler)
    // [修改] 将 converter 注入到 WorkflowHandler
    auto workflow_impl = std::make_shared<WorkflowHandler>(
        db_path.string(), 
        app_config_, 
        output_root_path_,
        disk_fs,
        notifier,
        serializer, 
        converter // [新增注入]
    );
    app_context_->workflow_handler = workflow_impl;

    // 8. 准备报表相关的依赖
    const std::string command = parser_.get_command();
    
    if (command == "query" || command == "export") {
        try {
            if (!db_manager_->check_and_open()) {
                 notifier->notify_error("错误: 数据库文件不存在: " + db_path.string());
                 throw std::runtime_error("Database missing");
            }
        } catch (const std::exception& e) {
            notifier->notify_error("Database Error: " + std::string(e.what()));
            throw;
        }
    }
    
    sqlite3* db_connection = db_manager_->get_db_connection();
    
    auto report_repo = std::make_shared<infrastructure::persistence::SqliteReportRepository>(
        db_connection, 
        app_config_
    );

    auto report_generator = std::make_unique<ReportGenerator>(report_repo);
    auto exporter = std::make_unique<Exporter>(exported_files_path_, disk_fs, notifier);
    
    auto report_impl = std::make_shared<ReportHandler>(
        std::move(report_generator), 
        std::move(exporter)
    );
    app_context_->report_handler = report_impl;
}

CliApplication::~CliApplication() = default;

void CliApplication::execute() {
    bool request_help = parser_.has_flag({"--help", "-h"});
    std::string command_name;
    try {
        command_name = parser_.get_command();
    } catch (...) {
        request_help = true;
    }

    if (request_help) {
        auto commands = CommandRegistry<AppContext>::Instance().CreateAllCommands(*app_context_);
        print_full_usage(parser_.get_raw_arg(0).c_str(), commands);
        return;
    }

    auto command = CommandRegistry<AppContext>::Instance().CreateCommand(command_name, *app_context_);

    if (command) {
        try {
            command->execute(parser_);
        } catch (const std::exception& e) {
            if (app_context_->user_notifier) {
                app_context_->user_notifier->notify_error("Error executing command '" + command_name + "': " + e.what());
            } else {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    } else {
        if (app_context_->user_notifier) {
             app_context_->user_notifier->notify_error("Unknown command '" + command_name + "'.");
             app_context_->user_notifier->notify_info("Run with --help to see available commands.");
        }
    }
}

void CliApplication::initialize_output_paths() {}