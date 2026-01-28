// cli/impl/commands/all_commands.cpp
// 合并�? pipeline_commands.cpp, export_command.cpp, query_command.cpp
#include "cli/impl/commands/all_commands.hpp"
#include "application/pipeline/context/pipeline_context.hpp"
#include "application/pipeline/factory.hpp"
#include "cli/framework/command.hpp"
#include "cli/framework/command_registry.hpp"
#include "cli/framework/console_io.hpp"
#include "cli/impl/app/app_context.hpp"
#include "cli/impl/utils/arg_utils.hpp"
#include "common/app_options.hpp"
#include "common/utils/time_utils.hpp"
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>

// ============================================================================
// 命令注册
// ============================================================================

[[maybe_unused]] static CommandRegistrar<AppContext>
    reg_convert("convert", [](AppContext &ctx) {
      if (!ctx.workflow_handler_)
        throw std::runtime_error("WorkflowHandler not initialized");
      return std::make_unique<ConvertCommand>(*ctx.workflow_handler_);
    });

[[maybe_unused]] static CommandRegistrar<AppContext>
    reg_import("import", [](AppContext &ctx) {
      if (!ctx.workflow_handler_)
        throw std::runtime_error("WorkflowHandler not initialized");
      return std::make_unique<ImportCommand>(*ctx.workflow_handler_);
    });

[[maybe_unused]] static CommandRegistrar<AppContext>
    reg_ingest("ingest", [](AppContext &ctx) {
      if (!ctx.workflow_handler_)
        throw std::runtime_error("WorkflowHandler not initialized");
      return std::make_unique<IngestCommand>(*ctx.workflow_handler_);
    });

[[maybe_unused]] static CommandRegistrar<AppContext>
    reg_val_logic("validate-logic", [](AppContext &ctx) {
      if (!ctx.serializer_ || !ctx.log_converter_)
        throw std::runtime_error("Dependencies not initialized");
      return std::make_unique<ValidateLogicCommand>(
          ctx.config_, ctx.config_.export_path_.value_or("./"),
          ctx.file_system_, ctx.user_notifier_, ctx.serializer_,
          ctx.log_converter_);
    });

[[maybe_unused]] static CommandRegistrar<AppContext>
    reg_val_struct("validate-structure", [](AppContext &ctx) {
      if (!ctx.serializer_ || !ctx.log_converter_)
        throw std::runtime_error("Dependencies not initialized");
      return std::make_unique<ValidateStructureCommand>(
          ctx.config_, ctx.config_.export_path_.value_or("./"),
          ctx.file_system_, ctx.user_notifier_, ctx.serializer_,
          ctx.log_converter_);
    });

[[maybe_unused]] static CommandRegistrar<AppContext>
    reg_export("export", [](AppContext &ctx) {
      return std::make_unique<ExportCommand>(ctx.report_handler_);
    });

[[maybe_unused]] static CommandRegistrar<AppContext>
    reg_query("query", [](AppContext &ctx) {
      return std::make_unique<QueryCommand>(ctx.report_handler_);
    });

// ============================================================================
// ConvertCommand 实现
// ============================================================================

ConvertCommand::ConvertCommand(IWorkflowHandler &workflow_handler)
    : workflow_handler_(workflow_handler) {}

std::vector<ArgDef> ConvertCommand::GetDefinitions() const {
  return {{"path", ArgType::Positional, {}, "Source file path", true, "", 0}};
}

std::string ConvertCommand::GetHelp() const {
  return "Transform raw log files (e.g., .txt) into structured JSON data.";
}

void ConvertCommand::Execute(const CommandParser &parser) {
  ParsedArgs args = CommandValidator::Validate(parser, GetDefinitions());
  AppOptions options;
  options.validate_structure_ = true;
  options.convert_ = true;
  options.save_processed_output_ = true;
  options.validate_logic_ = true;
  options.date_check_mode_ = DateCheckMode::Continuity;
  workflow_handler_.RunConverter(args.Get("path"), options);
}

// ============================================================================
// ImportCommand 实现
// ============================================================================

ImportCommand::ImportCommand(IWorkflowHandler &workflow_handler)
    : workflow_handler_(workflow_handler) {}

std::vector<ArgDef> ImportCommand::GetDefinitions() const {
  return {{"path",
           ArgType::Positional,
           {},
           "Directory path containing JSON files",
           true,
           "",
           0}};
}

std::string ImportCommand::GetHelp() const {
  return "Load structured JSON data into the database.";
}

void ImportCommand::Execute(const CommandParser &parser) {
  ParsedArgs args = CommandValidator::Validate(parser, GetDefinitions());
  ConsoleInput prompter;
  if (prompter.PromptConfirmation(
          "Are you sure you want to write to the database?")) {
    workflow_handler_.RunDatabaseImport(args.Get("path"));
  }
}

// ============================================================================
// IngestCommand 实现
// ============================================================================

IngestCommand::IngestCommand(IWorkflowHandler &workflow_handler)
    : workflow_handler_(workflow_handler) {}

std::vector<ArgDef> IngestCommand::GetDefinitions() const {
  return {{"path", ArgType::Positional, {}, "Source directory", true, "", 0},
          {"date_check",
           ArgType::Option,
           {"--date-check"},
           "Date check mode",
           false,
           ""},
          {"no_date_check",
           ArgType::Flag,
           {"--no-date-check"},
           "Disable date check",
           false,
           ""},
          {"save",
           ArgType::Flag,
           {"--save-processed"},
           "Save processed JSON",
           false,
           ""},
          {"no_save",
           ArgType::Flag,
           {"--no-save"},
           "Do not save processed JSON",
           false,
           ""}};
}

std::string IngestCommand::GetHelp() const {
  return "Full pipeline: Transform raw logs and load them into the database.";
}

void IngestCommand::Execute(const CommandParser &parser) {
  ParsedArgs args = CommandValidator::Validate(parser, GetDefinitions());
  DateCheckMode mode =
      args.Has("date_check")
          ? ArgUtils::ParseDateCheckMode(args.Get("date_check"))
          : (args.Has("no_date_check") ? DateCheckMode::None
                                       : DateCheckMode::Continuity);
  bool save_json = !args.Has("no_save");
  workflow_handler_.RunIngest(args.Get("path"), mode, save_json);
}

// ============================================================================
// ValidateLogicCommand 实现
// ============================================================================

ValidateLogicCommand::ValidateLogicCommand(
    const AppConfig &config, const std::filesystem::path &output_root,
    std::shared_ptr<core::interfaces::IFileSystem> fs,
    std::shared_ptr<core::interfaces::IUserNotifier> notifier,
    std::shared_ptr<core::interfaces::ILogSerializer> serializer,
    std::shared_ptr<core::interfaces::ILogConverter> converter)
    : app_config_(config), output_root_(output_root), fs_(std::move(fs)),
      notifier_(std::move(notifier)), serializer_(std::move(serializer)),
      converter_(std::move(converter)) {}

std::vector<ArgDef> ValidateLogicCommand::GetDefinitions() const {
  return {
      {"path", ArgType::Positional, {}, "Source directory path", true, "", 0},
      {"date_check",
       ArgType::Option,
       {"--date-check"},
       "Date check mode",
       false,
       "none"},
      {"no_date_check",
       ArgType::Flag,
       {"--no-date-check"},
       "Disable date check",
       false,
       ""}};
}

std::string ValidateLogicCommand::GetHelp() const {
  return "Verify business logic (e.g., date continuity) in source files.";
}

void ValidateLogicCommand::Execute(const CommandParser &parser) {
  auto args = CommandValidator::Validate(parser, GetDefinitions());
  AppOptions options;
  options.input_path_ = args.Get("path");
  options.convert_ = true;
  options.validate_logic_ = true;
  options.date_check_mode_ =
      args.Has("no_date_check")
          ? DateCheckMode::None
          : ArgUtils::ParseDateCheckMode(args.Get("date_check"));

  core::pipeline::PipelineContext context(app_config_, output_root_, fs_,
                                          notifier_);
  context.config.input_root_ = options.input_path_;
  auto pipeline = core::pipeline::PipelineFactory::CreateIngestPipeline(
      options, app_config_, serializer_, converter_);
  pipeline->Run(std::move(context));
}

// ============================================================================
// ValidateStructureCommand 实现
// ============================================================================

ValidateStructureCommand::ValidateStructureCommand(
    const AppConfig &config, const std::filesystem::path &output_root,
    std::shared_ptr<core::interfaces::IFileSystem> fs,
    std::shared_ptr<core::interfaces::IUserNotifier> notifier,
    std::shared_ptr<core::interfaces::ILogSerializer> serializer,
    std::shared_ptr<core::interfaces::ILogConverter> converter)
    : app_config_(config), output_root_(output_root), fs_(std::move(fs)),
      notifier_(std::move(notifier)), serializer_(std::move(serializer)),
      converter_(std::move(converter)) {}

std::vector<ArgDef> ValidateStructureCommand::GetDefinitions() const {
  return {{"path",
           ArgType::Positional,
           {},
           "Source directory or file path",
           true,
           "",
           0}};
}

std::string ValidateStructureCommand::GetHelp() const {
  return "Check source files for syntax and formatting errors.";
}

void ValidateStructureCommand::Execute(const CommandParser &parser) {
  auto args = CommandValidator::Validate(parser, GetDefinitions());
  AppOptions options;
  options.input_path_ = args.Get("path");
  options.validate_structure_ = true;
  core::pipeline::PipelineContext context(app_config_, output_root_, fs_,
                                          notifier_);
  context.config.input_root_ = options.input_path_;
  auto pipeline = core::pipeline::PipelineFactory::CreateIngestPipeline(
      options, app_config_, serializer_, converter_);
  pipeline->Run(std::move(context));
}

// ============================================================================
// ExportCommand 实现
// ============================================================================

ExportCommand::ExportCommand(std::shared_ptr<IReportHandler> report_handler)
    : report_handler_(std::move(report_handler)) {}

std::vector<ArgDef> ExportCommand::GetDefinitions() const {
  return {{"type",
           ArgType::Positional,
           {},
           "Export scope: day, month, week, year, recent, range, "
           "all-day, all-month, all-week, all-year, all-recent",
           true,
           "",
           0},
          {"argument",
           ArgType::Positional,
           {},
           "Date, Month, Year, StartDate, or Days",
           false,
           "",
           1},
          {"week_num",
           ArgType::Positional,
           {},
           "Week number or EndDate (for 'week' or 'range' type)",
           false,
           "",
           2},
          {"format",
           ArgType::Option,
           {"-f", "--format"},
           "Output format",
           false,
           "md"},
          {"output",
           ArgType::Option,
           {"-o", "--output"},
           "Output directory",
           false,
           ""},
          {"db",
           ArgType::Option,
           {"--db", "--database"},
           "Database path",
           false,
           ""}};
}

std::string ExportCommand::GetHelp() const {
  return "Export statistical reports (daily, weekly, etc.) to files.";
}

void ExportCommand::Execute(const CommandParser &parser) {
  if (!report_handler_) {
    throw std::runtime_error(
        "ReportHandler not initialized. (Database might be missing)");
  }

  ParsedArgs args = CommandValidator::Validate(parser, GetDefinitions());

  std::string sub_command = args.Get("type");
  std::string export_arg = args.Get("argument");
  std::string week_arg = args.Get("week_num");

  std::vector<ReportFormat> formats;
  if (args.Has("format")) {
    formats = ArgUtils::ParseReportFormats(args.Get("format"));
  } else {
    formats = {ReportFormat::Markdown};
  }

  for (const auto &format : formats) {
    if (sub_command == "day" || sub_command == "month" ||
        sub_command == "recent" || sub_command == "all-recent" ||
        sub_command == "week" || sub_command == "year" ||
        sub_command == "range") {
      if (export_arg.empty()) {
        throw std::runtime_error("Argument required for export type '" +
                                 sub_command + "'.");
      }

      if (sub_command == "day") {
        std::string date_str = TimeUtils::NormalizeToDateFormat(export_arg);
        report_handler_->RunExportSingleDayReport(date_str, format);
      } else if (sub_command == "month") {
        std::string month_str = TimeUtils::NormalizeToMonthFormat(export_arg);
        report_handler_->RunExportSingleMonthReport(month_str, format);
      } else if (sub_command == "week") {
        if (week_arg.empty()) {
          throw std::runtime_error(
              "Week number required for export type 'week'.");
        }
        try {
          int year = std::stoi(export_arg);
          int week = std::stoi(week_arg);
          report_handler_->RunExportSingleWeekReport(year, week, format);
        } catch (const std::exception &) {
          throw std::runtime_error("Invalid year or week number.");
        }
      } else if (sub_command == "recent") {
        try {
          report_handler_->RunExportSingleRecentReport(std::stoi(export_arg),
                                                       format);
        } catch (const std::exception &) {
          throw std::runtime_error(
              "Invalid number provided for 'export recent': " + export_arg);
        }
      } else if (sub_command == "range") {
        if (week_arg.empty()) {
          throw std::runtime_error("End date required for 'export range'. "
                                   "Usage: export range <start> <end>");
        }
        std::string start_normalized =
            TimeUtils::NormalizeRangeStart(export_arg);
        std::string end_normalized = TimeUtils::NormalizeRangeEnd(week_arg);
        report_handler_->RunExportSingleRangeReport(start_normalized,
                                                    end_normalized, format);
      } else if (sub_command == "year") {
        try {
          report_handler_->RunExportSingleYearReport(std::stoi(export_arg),
                                                     format);
        } catch (const std::exception &) {
          throw std::runtime_error("Invalid year provided: " + export_arg);
        }
      } else if (sub_command == "all-recent") {
        std::vector<int> days_list;
        std::string token;
        std::istringstream tokenStream(export_arg);
        while (std::getline(tokenStream, token, ',')) {
          try {
            days_list.push_back(std::stoi(token));
          } catch (const std::exception &) {
            throw std::runtime_error(
                "Invalid number in days list for 'export all-recent': " +
                token);
          }
        }
        report_handler_->RunExportAllRecentReportsQuery(days_list, format);
      }
    } else if (sub_command == "all-day") {
      report_handler_->RunExportAllDailyReportsQuery(format);
    } else if (sub_command == "all-month") {
      report_handler_->RunExportAllMonthlyReportsQuery(format);
    } else if (sub_command == "all-year") {
      report_handler_->RunExportAllYearlyReportsQuery(format);
    } else if (sub_command == "all-week") {
      report_handler_->RunExportAllWeeklyReportsQuery(format);
    } else {
      throw std::runtime_error("Unknown export type '" + sub_command + "'.");
    }
  }
}

// ============================================================================
// QueryCommand 实现
// ============================================================================

QueryCommand::QueryCommand(std::shared_ptr<IReportHandler> report_handler)
    : report_handler_(std::move(report_handler)) {}

std::vector<ArgDef> QueryCommand::GetDefinitions() const {
  return {
      {"type",
       ArgType::Positional,
       {},
       "Query scope: day, month, week, year, recent, range",
       true,
       "",
       0},
      {"argument",
       ArgType::Positional,
       {},
       "Date (YYYY-MM-DD), Month (YYYY-MM), Year (YYYY), StartDate, or Days",
       true,
       "",
       1},
      {"week_num",
       ArgType::Positional,
       {},
       "Week number or EndDate (required for 'week' or 'range' type)",
       false,
       "",
       2},
      {"format",
       ArgType::Option,
       {"-f", "--format"},
       "Output format (md, tex, typ)",
       false,
       "md"}};
}

std::string QueryCommand::GetHelp() const {
  return "Search and display statistics from the database.";
}

void QueryCommand::Execute(const CommandParser &parser) {
  if (!report_handler_) {
    throw std::runtime_error(
        "Database service not initialized. (Check if database file exists)");
  }

  ParsedArgs args = CommandValidator::Validate(parser, GetDefinitions());

  std::string sub_command = args.Get("type");
  std::string query_arg = args.Get("argument");
  std::string format_str = args.Get("format");
  std::string week_arg = args.Get("week_num");

  std::vector<ReportFormat> formats = ArgUtils::ParseReportFormats(format_str);

  if (sub_command == "day") {
    query_arg = TimeUtils::NormalizeToDateFormat(query_arg);
  } else if (sub_command == "month") {
    query_arg = TimeUtils::NormalizeToMonthFormat(query_arg);
  }

  for (size_t i = 0; i < formats.size(); ++i) {
    ReportFormat format = formats[i];

    if (i > 0) {
      std::cout << "\n" << std::string(40, '=') << "\n";
    }

    if (sub_command == "day") {
      std::cout << report_handler_->RunDailyQuery(query_arg, format);
    } else if (sub_command == "month") {
      std::cout << report_handler_->RunMonthlyQuery(query_arg, format);
    } else if (sub_command == "week") {
      if (week_arg.empty()) {
        throw std::runtime_error("Week number is required for 'week' query. "
                                 "Usage: query week <year> <week_num>");
      }
      try {
        int year = std::stoi(query_arg);
        int week = std::stoi(week_arg);
        std::cout << report_handler_->RunWeeklyQuery(year, week, format);
      } catch (const std::exception &) {
        throw std::runtime_error("Invalid year or week number.");
      }
    } else if (sub_command == "year") {
      try {
        int year = std::stoi(query_arg);
        std::cout << report_handler_->RunYearlyQuery(year, format);
      } catch (const std::exception &) {
        throw std::runtime_error("Invalid year number.");
      }
    } else if (sub_command == "recent") {
      std::vector<int> recent_days_list;
      std::string token;
      std::istringstream tokenStream(query_arg);

      while (std::getline(tokenStream, token, ',')) {
        try {
          token.erase(0, token.find_first_not_of(" \t\n\r"));
          token.erase(token.find_last_not_of(" \t\n\r") + 1);

          if (!token.empty()) {
            recent_days_list.push_back(std::stoi(token));
          }
        } catch (const std::exception &) {
          std::cerr << "\033[31mError: \033[0mInvalid number '" << token
                    << "' in list. Skipping.\n";
        }
      }

      if (!recent_days_list.empty()) {
        std::cout << report_handler_->RunRecentQueries(recent_days_list,
                                                       format);
      }
    } else if (sub_command == "range") {
      if (week_arg.empty()) {
        throw std::runtime_error("End date is required for 'range' query. "
                                 "Usage: query range <start_date> <end_date>");
      }
      std::string start_normalized = TimeUtils::NormalizeRangeStart(query_arg);
      std::string end_normalized = TimeUtils::NormalizeRangeEnd(week_arg);
      std::cout << report_handler_->RunRangeQuery(start_normalized,
                                                  end_normalized, format);
    } else {
      throw std::runtime_error("Unknown query type '" + sub_command +
                               "'. Supported: day, week, month, year, "
                               "recent, range.");
    }
  }
}
