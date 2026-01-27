// cli/impl/commands/all_commands.cpp
// 合并�? pipeline_commands.cpp, export_command.cpp, query_command.cpp
#include "cli/impl/commands/all_commands.hpp"
#include "cli/framework/command.hpp"
#include "cli/framework/command_registry.hpp"
#include "cli/framework/console_io.hpp"
#include "cli/impl/app/app_context.hpp"
#include "cli/impl/utils/arg_utils.hpp"
#include "common/app_options.hpp"
#include "common/utils/time_utils.hpp"
#include "application/pipeline_factory.hpp"
#include "application/pipeline/context/pipeline_context.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>

// ============================================================================
// 命令注册
// ============================================================================

static CommandRegistrar<AppContext> reg_convert("convert", [](AppContext& ctx) {
    if (!ctx.workflow_handler) throw std::runtime_error("WorkflowHandler not initialized");
    return std::make_unique<ConvertCommand>(*ctx.workflow_handler);
});

static CommandRegistrar<AppContext> reg_import("import", [](AppContext& ctx) {
    if (!ctx.workflow_handler) throw std::runtime_error("WorkflowHandler not initialized");
    return std::make_unique<ImportCommand>(*ctx.workflow_handler);
});

static CommandRegistrar<AppContext> reg_ingest("ingest", [](AppContext& ctx) {
    if (!ctx.workflow_handler) throw std::runtime_error("WorkflowHandler not initialized");
    return std::make_unique<IngestCommand>(*ctx.workflow_handler);
});

static CommandRegistrar<AppContext> reg_val_logic("validate-logic", [](AppContext& ctx) {
    if (!ctx.serializer || !ctx.log_converter) throw std::runtime_error("Dependencies not initialized");
    return std::make_unique<ValidateLogicCommand>(
        ctx.config, ctx.config.export_path.value_or("./"), 
        ctx.file_system, ctx.user_notifier, ctx.serializer, ctx.log_converter);
});

static CommandRegistrar<AppContext> reg_val_struct("validate-structure", [](AppContext& ctx) {
    if (!ctx.serializer || !ctx.log_converter) throw std::runtime_error("Dependencies not initialized");
    return std::make_unique<ValidateStructureCommand>(
        ctx.config, ctx.config.export_path.value_or("./"), 
        ctx.file_system, ctx.user_notifier, ctx.serializer, ctx.log_converter);
});

static CommandRegistrar<AppContext> reg_export("export", [](AppContext& ctx) {
    return std::make_unique<ExportCommand>(ctx.report_handler);
});

static CommandRegistrar<AppContext> reg_query("query", [](AppContext& ctx) {
    return std::make_unique<QueryCommand>(ctx.report_handler);
});

// ============================================================================
// ConvertCommand 实现
// ============================================================================

ConvertCommand::ConvertCommand(IWorkflowHandler& workflow_handler) : workflow_handler_(workflow_handler) {}

std::vector<ArgDef> ConvertCommand::get_definitions() const {
    return {{"path", ArgType::Positional, {}, "Source file path", true, "", 0}};
}

std::string ConvertCommand::get_help() const { 
    return "Converts source files (e.g., .txt) to processed JSON format."; 
}

void ConvertCommand::execute(const CommandParser& parser) {
    ParsedArgs args = CommandValidator::validate(parser, get_definitions());
    AppOptions options;
    options.validate_structure = true; options.convert = true; options.save_processed_output = true;
    options.validate_logic = true; options.date_check_mode = DateCheckMode::Continuity;
    workflow_handler_.run_converter(args.get("path"), options);
}

// ============================================================================
// ImportCommand 实现
// ============================================================================

ImportCommand::ImportCommand(IWorkflowHandler& workflow_handler) : workflow_handler_(workflow_handler) {}

std::vector<ArgDef> ImportCommand::get_definitions() const {
    return {{"path", ArgType::Positional, {}, "Directory path containing JSON files", true, "", 0}};
}

std::string ImportCommand::get_help() const { 
    return "Imports processed JSON data into the database."; 
}

void ImportCommand::execute(const CommandParser& parser) {
    ParsedArgs args = CommandValidator::validate(parser, get_definitions());
    ConsoleInput prompter;
    if (prompter.prompt_confirmation("Are you sure you want to write to the database?")) {
        workflow_handler_.run_database_import(args.get("path"));
    }
}

// ============================================================================
// IngestCommand 实现
// ============================================================================

IngestCommand::IngestCommand(IWorkflowHandler& workflow_handler) : workflow_handler_(workflow_handler) {}

std::vector<ArgDef> IngestCommand::get_definitions() const {
    return {
        {"path", ArgType::Positional, {}, "Source directory", true, "", 0},
        {"date_check", ArgType::Option, {"--date-check"}, "Date check mode", false, ""},
        {"no_date_check", ArgType::Flag, {"--no-date-check"}, "Disable date check", false, ""},
        {"save", ArgType::Flag, {"--save-processed"}, "Save processed JSON", false, ""},
        {"no_save", ArgType::Flag, {"--no-save"}, "Do not save processed JSON", false, ""}
    };
}

std::string IngestCommand::get_help() const { 
    return "Runs the full data ingestion pipeline."; 
}

void IngestCommand::execute(const CommandParser& parser) {
    ParsedArgs args = CommandValidator::validate(parser, get_definitions());
    DateCheckMode mode = args.has("date_check") ? ArgUtils::parse_date_check_mode(args.get("date_check")) : 
                         (args.has("no_date_check") ? DateCheckMode::None : DateCheckMode::Continuity);
    bool save_json = !args.has("no_save");
    workflow_handler_.run_ingest(args.get("path"), mode, save_json);
}

// ============================================================================
// ValidateLogicCommand 实现
// ============================================================================

ValidateLogicCommand::ValidateLogicCommand(const AppConfig& config, const std::filesystem::path& output_root,
    std::shared_ptr<core::interfaces::IFileSystem> fs, std::shared_ptr<core::interfaces::IUserNotifier> notifier,
    std::shared_ptr<core::interfaces::ILogSerializer> serializer, std::shared_ptr<core::interfaces::ILogConverter> converter)
    : app_config_(config), output_root_(output_root), fs_(std::move(fs)), notifier_(std::move(notifier)), 
      serializer_(std::move(serializer)), converter_(std::move(converter)) {}

std::vector<ArgDef> ValidateLogicCommand::get_definitions() const {
    return {
        {"path", ArgType::Positional, {}, "Source directory path", true, "", 0},
        {"date_check", ArgType::Option, {"--date-check"}, "Date check mode", false, "none"},
        {"no_date_check", ArgType::Flag, {"--no-date-check"}, "Disable date check", false, ""}
    };
}

std::string ValidateLogicCommand::get_help() const { 
    return "Validates business logic (e.g., date continuity)."; 
}

void ValidateLogicCommand::execute(const CommandParser& parser) {
    auto args = CommandValidator::validate(parser, get_definitions());
    AppOptions options;
    options.input_path = args.get("path"); options.convert = true; options.validate_logic = true;
    options.date_check_mode = args.has("no_date_check") ? DateCheckMode::None : 
                             ArgUtils::parse_date_check_mode(args.get("date_check"));

    core::pipeline::PipelineContext context(app_config_, output_root_, fs_, notifier_);
    context.config.input_root = options.input_path;
    auto pipeline = core::pipeline::PipelineFactory::create_ingest_pipeline(options, app_config_, serializer_, converter_);
    pipeline->run(std::move(context));
}

// ============================================================================
// ValidateStructureCommand 实现
// ============================================================================

ValidateStructureCommand::ValidateStructureCommand(const AppConfig& config, const std::filesystem::path& output_root,
    std::shared_ptr<core::interfaces::IFileSystem> fs, std::shared_ptr<core::interfaces::IUserNotifier> notifier,
    std::shared_ptr<core::interfaces::ILogSerializer> serializer, std::shared_ptr<core::interfaces::ILogConverter> converter)
    : app_config_(config), output_root_(output_root), fs_(std::move(fs)), notifier_(std::move(notifier)), 
      serializer_(std::move(serializer)), converter_(std::move(converter)) {}

std::vector<ArgDef> ValidateStructureCommand::get_definitions() const {
    return {{"path", ArgType::Positional, {}, "Source directory or file path", true, "", 0}};
}

std::string ValidateStructureCommand::get_help() const { 
    return "Validates the syntax and structure of source TXT files."; 
}

void ValidateStructureCommand::execute(const CommandParser& parser) {
    auto args = CommandValidator::validate(parser, get_definitions());
    AppOptions options;
    options.input_path = args.get("path"); options.validate_structure = true;
    core::pipeline::PipelineContext context(app_config_, output_root_, fs_, notifier_);
    context.config.input_root = options.input_path;
    auto pipeline = core::pipeline::PipelineFactory::create_ingest_pipeline(options, app_config_, serializer_, converter_);
    pipeline->run(std::move(context));
}

// ============================================================================
// ExportCommand 实现
// ============================================================================

ExportCommand::ExportCommand(std::shared_ptr<IReportHandler> report_handler)
    : report_handler_(std::move(report_handler)) {}

std::vector<ArgDef> ExportCommand::get_definitions() const {
    return {
        {"type", ArgType::Positional, {}, "Export type (daily, monthly, week, period, all-daily...)", true, "", 0},
        {"argument", ArgType::Positional, {}, "Date, Year, or Period", false, "", 1},
        {"week_num", ArgType::Positional, {}, "Week number (for 'week' type)", false, "", 2},
        {"format", ArgType::Option, {"-f", "--format"}, "Output format", false, "md"},
        {"output", ArgType::Option, {"-o", "--output"}, "Output directory", false, ""},
        {"db", ArgType::Option, {"--db", "--database"}, "Database path", false, ""}
    };
}

std::string ExportCommand::get_help() const {
    return "Exports reports (daily, weekly, monthly, period, etc.) to files.";
}

void ExportCommand::execute(const CommandParser& parser) {
    if (!report_handler_) {
        throw std::runtime_error("ReportHandler not initialized. (Database might be missing)");
    }

    ParsedArgs args = CommandValidator::validate(parser, get_definitions());

    std::string sub_command = args.get("type");
    std::string export_arg = args.get("argument"); 
    std::string week_arg = args.get("week_num");
    
    std::vector<ReportFormat> formats;
    if (args.has("format")) {
        formats = ArgUtils::parse_report_formats(args.get("format"));
    } else {
        formats = {ReportFormat::Markdown};
    }

    for (const auto& format : formats) {
        if (sub_command == "daily" || sub_command == "monthly" || sub_command == "period" || sub_command == "all-period" || sub_command == "week") {
            if (export_arg.empty()) {
                throw std::runtime_error("Argument required for export type '" + sub_command + "'.");
            }

            if (sub_command == "daily") {
                std::string date_str = TimeUtils::normalize_to_date_format(export_arg);
                report_handler_->run_export_single_day_report(date_str, format);
            } else if (sub_command == "monthly") {
                std::string month_str = TimeUtils::normalize_to_month_format(export_arg);
                report_handler_->run_export_single_month_report(month_str, format);
            } 
            else if (sub_command == "week") {
                if (week_arg.empty()) {
                     throw std::runtime_error("Week number required for export type 'week'.");
                }
                try {
                    int year = std::stoi(export_arg);
                    int week = std::stoi(week_arg);
                    report_handler_->run_export_single_week_report(year, week, format);
                } catch (const std::exception&) {
                    throw std::runtime_error("Invalid year or week number.");
                }
            }
            else if (sub_command == "period") {
                try {
                    report_handler_->run_export_single_period_report(std::stoi(export_arg), format);
                } catch (const std::exception&) {
                    throw std::runtime_error("Invalid number provided for 'export period': " + export_arg);
                }
            } else if (sub_command == "all-period") {
                std::vector<int> days_list;
                std::string token;
                std::istringstream tokenStream(export_arg);
                while (std::getline(tokenStream, token, ',')) {
                    try {
                        days_list.push_back(std::stoi(token));
                    } catch (const std::exception&) {
                        throw std::runtime_error("Invalid number in days list for 'export all-period': " + token);
                    }
                }
                report_handler_->run_export_all_period_reports_query(days_list, format);
            }
        }
        else if (sub_command == "all-daily") {
            report_handler_->run_export_all_daily_reports_query(format);
        } else if (sub_command == "all-monthly") {
            report_handler_->run_export_all_monthly_reports_query(format);
        } 
        else if (sub_command == "all-week" || sub_command == "all-weekly") {
            report_handler_->run_export_all_weekly_reports_query(format);
        }
        else {
            throw std::runtime_error("Unknown export type '" + sub_command + "'.");
        }
    }
}

// ============================================================================
// QueryCommand 实现
// ============================================================================

QueryCommand::QueryCommand(std::shared_ptr<IReportHandler> report_handler)
    : report_handler_(std::move(report_handler)) {}

std::vector<ArgDef> QueryCommand::get_definitions() const {
    return {
        {"type", ArgType::Positional, {}, "Query type (daily, monthly, week, period)", true, "", 0},
        {"argument", ArgType::Positional, {}, "Date (YYYY-MM-DD), Month (YYYY-MM), Year (YYYY), or Period (days)", true, "", 1},
        {"week_num", ArgType::Positional, {}, "Week number (required for 'week' type)", false, "", 2},
        {"format", ArgType::Option, {"-f", "--format"}, "Output format (md, tex, typ)", false, "md"}
    };
}

std::string QueryCommand::get_help() const {
    return "Queries statistics (daily, weekly, monthly, period) from the database.";
}

void QueryCommand::execute(const CommandParser& parser) {
    if (!report_handler_) {
        throw std::runtime_error("Database service not initialized. (Check if database file exists)");
    }

    ParsedArgs args = CommandValidator::validate(parser, get_definitions());

    std::string sub_command = args.get("type");
    std::string query_arg = args.get("argument");
    std::string format_str = args.get("format"); 
    std::string week_arg = args.get("week_num");

    std::vector<ReportFormat> formats = ArgUtils::parse_report_formats(format_str);

    if (sub_command == "daily") {
        query_arg = TimeUtils::normalize_to_date_format(query_arg);
    } else if (sub_command == "monthly") {
        query_arg = TimeUtils::normalize_to_month_format(query_arg);
    } 

    for (size_t i = 0; i < formats.size(); ++i) {
        ReportFormat format = formats[i];
        
        if (i > 0) {
            std::cout << "\n" << std::string(40, '=') << "\n";
        }

        if (sub_command == "daily") {
            std::cout << report_handler_->run_daily_query(query_arg, format);
        } 
        else if (sub_command == "monthly") {
            std::cout << report_handler_->run_monthly_query(query_arg, format);
        } 
        else if (sub_command == "week") {
            if (week_arg.empty()) {
                throw std::runtime_error("Week number is required for 'week' query. Usage: query week <year> <week_num>");
            }
            try {
                int year = std::stoi(query_arg);
                int week = std::stoi(week_arg);
                std::cout << report_handler_->run_weekly_query(year, week, format);
            } catch (const std::exception&) {
                throw std::runtime_error("Invalid year or week number.");
            }
        }
        else if (sub_command == "period") {
            std::vector<int> periods;
            std::string token;
            std::istringstream tokenStream(query_arg);
            
            while (std::getline(tokenStream, token, ',')) {
                try {
                    token.erase(0, token.find_first_not_of(" \t\n\r"));
                    token.erase(token.find_last_not_of(" \t\n\r") + 1);
                    
                    if (!token.empty()) {
                        periods.push_back(std::stoi(token));
                    }
                } catch (const std::exception&) {
                    std::cerr << "\033[31mError: \033[0mInvalid number '" << token << "' in list. Skipping.\n";
                }
            }

            if (!periods.empty()) {
                std::cout << report_handler_->run_period_queries(periods, format);
            }
        } else {
            throw std::runtime_error("Unknown query type '" + sub_command + "'. Supported: daily, week, monthly, period.");
        }
    }
}
