// cli/impl/app/cli_application.cpp
#include "cli/impl/app/cli_application.hpp"
#include "cli/framework/console_io.hpp"
#include "cli/impl/app/app_context.hpp"
#include "cli/impl/utils/help_formatter.hpp"
#include "core/infrastructure/persistence/sqlite_report_repository_adapter.hpp"
#include "io/disk_file_system.hpp"

// Implementation Includes
#include "application/service/report_generator.hpp"
#include "application/service/report_handler.hpp"
#include "application/service/workflow_handler.hpp"
#include "cli/framework/command_registry.hpp"
#include "config/config_loader.hpp"
#include "converter/log_processor.hpp"
#include "core/infrastructure/persistence/db_manager.hpp"
#include "core/infrastructure/reporting/exporter.hpp"
#include "serializer/json_serializer.hpp"

namespace fs = std::filesystem;
const std::string DATABASE_FILENAME = "time_data.sqlite3";

static ParserConfig get_app_parser_config() {
  return {{"-o", "--output", "-f", "--format", "--date-check", "--db",
           "--database"},
          {"--save-processed", "--no-save", "--no-date-check"}};
}

CliApplication::CliApplication(const std::vector<std::string> &args)
    : parser_(args, get_app_parser_config()) {

  // 0. 初始化服务容器
  app_context_ = std::make_shared<AppContext>();

  // 1. 路径初始化
  fs::path exe_path(parser_.GetRawArg(0));
  fs::path default_output_root =
      fs::absolute(exe_path.parent_path() / "output");

  fs::path db_path;
  if (auto user_db_path = parser_.GetOption({"--db", "--database"})) {
    db_path = fs::absolute(*user_db_path);
  } else {
    db_path = default_output_root / DATABASE_FILENAME;
  }

  if (auto path_opt = parser_.GetOption({"-o", "--output"})) {
    exported_files_path_ = fs::absolute(*path_opt);
  } else {
    exported_files_path_ = default_output_root / "exported_files";
  }
  output_root_path_ = default_output_root;

  // 2. 基础设施初始化
  auto disk_fs = std::make_shared<io::DiskFileSystem>();
  app_context_->file_system_ = disk_fs;

  auto notifier = std::make_shared<ConsoleNotifier>();
  app_context_->user_notifier_ = notifier;

  // 3. 插件初始(Serializer & Converter)
  auto serializer = std::make_shared<serializer::JsonSerializer>();
  app_context_->serializer_ = serializer;

  // 实例化具体的 Converter
  auto converter = std::make_shared<LogProcessor>();
  app_context_->log_converter_ = converter;

  // 4. 加载配置
  ConfigLoader config_loader(parser_.GetRawArg(0), disk_fs);
  app_config_ = config_loader.load_configuration();
  app_context_->config_ = app_config_;

  // 5. 初始化目录
  try {
    disk_fs->CreateDirectories(output_root_path_);
    disk_fs->CreateDirectories(exported_files_path_);
  } catch (const std::exception &e) {
    notifier->NotifyError(
        "Fatal Error: Failed to initialize output directories: " +
        std::string(e.what()));
    throw;
  }

  // 6. 初始化数据库管理器
  db_manager_ = std::make_unique<DBManager>(db_path.string());

  // 7. 初始Core Service (WorkflowHandler)
  auto workflow_impl = std::make_shared<WorkflowHandler>(
      db_path.string(), app_config_, output_root_path_, disk_fs, notifier,
      serializer, converter);
  app_context_->workflow_handler_ = workflow_impl;

  // 8. 准备报表相关的依    // [核心修复]
  // 仅当命令需要查导出时，才打开数据库并初始ReportRepository
  const std::string command = parser_.GetCommand();

  if (command == "query" || command == "export") {
    try {
      // 尝试打开数据库
      if (!db_manager_->CheckAndOpen()) {
        notifier->NotifyError("错误: 数据库文件不存在: " + db_path.string());
        throw std::runtime_error("Database missing");
      }

      // 获取连接（此时非空）
      sqlite3 *db_connection = db_manager_->GetDbConnection();

      // 初始Repository
      auto report_repo = std::make_shared<
          infrastructure::persistence::SqliteReportRepositoryAdapter>(
          db_connection, app_config_.loaded_reports_);

      auto report_generator = std::make_unique<ReportGenerator>(report_repo);
      auto exporter =
          std::make_unique<Exporter>(exported_files_path_, disk_fs, notifier);

      // [核心修改] 传入 app_config_.exe_dir_path_.string()
      auto report_impl = std::make_shared<ReportHandler>(
          std::move(report_generator), std::move(exporter),
          app_config_.exe_dir_path_.string() // <--- 新增参数
      );
      app_context_->report_handler_ = report_impl;

    } catch (const std::exception &e) {
      notifier->NotifyError("Database Error: " + std::string(e.what()));
      throw;
    }
  }
}

CliApplication::~CliApplication() = default;

void CliApplication::Execute() {
  bool request_help = parser_.HasFlag({"--help", "-h"});
  std::string command_name;
  try {
    command_name = parser_.GetCommand();
  } catch (...) {
    request_help = true;
  }

  if (request_help) {
    auto commands = CommandRegistry<AppContext>::Instance().CreateAllCommands(
        *app_context_);
    PrintFullUsage(parser_.GetRawArg(0).c_str(), commands);
    return;
  }

  auto command = CommandRegistry<AppContext>::Instance().CreateCommand(
      command_name, *app_context_);

  if (command) {
    try {
      command->Execute(parser_);
    } catch (const std::exception &e) {
      if (app_context_->user_notifier_) {
        app_context_->user_notifier_->NotifyError(
            "Error executing command '" + command_name + "': " + e.what());
      } else {
        std::cerr << "Error: " << e.what() << std::endl;
      }
    }
  } else {
    if (app_context_->user_notifier_) {
      app_context_->user_notifier_->NotifyError("Unknown command '" +
                                                command_name + "'.");
      app_context_->user_notifier_->NotifyInfo(
          "Run with --help to see available commands.");
    }
  }
}

void CliApplication::InitializeOutputPaths() {}
