// cli/framework/arg_parser.hpp
#ifndef CLI_FRAMEWORK_ARG_PARSER_HPP_
#define CLI_FRAMEWORK_ARG_PARSER_HPP_

#include "cli/framework/parsed_args.hpp"
#include "cli/framework/help_formatter.hpp" // 需要 Option 结构体和 HelpFormatter 类
#include <string>
#include <vector>
#include <optional>

namespace Cli::Framework {

    class ArgParser {
    public:
        explicit ArgParser(std::string prog_name);
        
        // 注册选项
        void add_option(const std::string& short_name, 
                        const std::string& long_name, 
                        const std::string& desc, 
                        bool requires_value = false);
        
        // 解析参数
        std::optional<ParsedArgs> parse(int argc, char* argv[]);
        
        // 打印帮助
        void print_help() const;
        
        // 打印错误
        void print_error(const std::string& msg) const;

    private:
        // 内部辅助函数
        const Option* find_option(const std::string& arg) const;

        HelpFormatter formatter_;
        std::vector<Option> defined_options_;
    };

}

#endif // CLI_FRAMEWORK_ARG_PARSER_HPP_