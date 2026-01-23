// cli/impl/log_generator_cli.hpp
#ifndef CLI_IMPL_LOG_GENERATOR_CLI_HPP_
#define CLI_IMPL_LOG_GENERATOR_CLI_HPP_

#include "common/config_types.hpp"
#include "cli/framework/arg_parser.hpp"
#include "cli/framework/parsed_args.hpp"
#include <optional>

namespace Cli::Impl {

    class LogGeneratorCli {
    public:
        std::optional<Config> parse(int argc, char* argv[]);

    private:
        // 定义应用支持的所有参数
        void setup_options(Framework::ArgParser& parser) const;

        // 处理通用命令 (help/version)
        bool handle_general_commands(const Framework::ArgParser& parser, 
                                     const Framework::ParsedArgs& args) const;
    };

}

#endif // CLI_IMPL_LOG_GENERATOR_CLI_HPP_