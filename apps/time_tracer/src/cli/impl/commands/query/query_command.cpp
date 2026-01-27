// cli/impl/commands/query/query_command.cpp
#include "cli/impl/commands/query/query_command.hpp"
#include "cli/framework/core/command_parser.hpp" 
#include "cli/framework/core/command_registry.hpp" 
#include "cli/framework/core/command_validator.hpp" 
#include "cli/impl/utils/arg_utils.hpp"
#include "common/utils/time_utils.hpp"
#include "cli/impl/app/app_context.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

static CommandRegistrar<AppContext> registrar("query", [](AppContext& ctx) {
    // [修改] 移除 if (!ctx.report_handler) check，直接传入指针
    // 即使 handler 为空，也允许创建 Command 对象以提供 Help 信息
    return std::make_unique<QueryCommand>(ctx.report_handler);
});

// [修改] 构造函数适配 shared_ptr
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
    // [新增] 运行时检查依赖
    if (!report_handler_) {
        throw std::runtime_error("Database service not initialized. (Check if database file exists)");
    }

    // 1. 统一验证与解析
    ParsedArgs args = CommandValidator::validate(parser, get_definitions());

    // 2. 获取参数
    std::string sub_command = args.get("type");
    std::string query_arg = args.get("argument");
    std::string format_str = args.get("format"); 
    std::string week_arg = args.get("week_num");

    // 3. 业务逻辑
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

        // [修改] 使用 -> 调用成员函数
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