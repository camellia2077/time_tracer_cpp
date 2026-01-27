// cli/impl/commands/export/export_command.cpp
#include "cli/impl/commands/export/export_command.hpp"
#include "cli/framework/core/command_parser.hpp"
#include "cli/framework/core/command_registry.hpp"
#include "cli/framework/core/command_validator.hpp" 
#include "cli/impl/utils/arg_utils.hpp"
#include "common/utils/time_utils.hpp"
#include "cli/impl/app/app_context.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>

static CommandRegistrar<AppContext> registrar("export", [](AppContext& ctx) {
    // [修改] 移除依赖检查，直接传递指针
    return std::make_unique<ExportCommand>(ctx.report_handler);
});

// [修改] 构造函数
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
    // [新增] 运行时检查
    if (!report_handler_) {
        throw std::runtime_error("ReportHandler not initialized. (Database might be missing)");
    }

    // 1. 统一验证
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

    // 2. 遍历所有请求的格式进行导出
    for (const auto& format : formats) {
        if (sub_command == "daily" || sub_command == "monthly" || sub_command == "period" || sub_command == "all-period" || sub_command == "week") {
            if (export_arg.empty()) {
                throw std::runtime_error("Argument required for export type '" + sub_command + "'.");
            }

            // [修改] 使用 -> 调用
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