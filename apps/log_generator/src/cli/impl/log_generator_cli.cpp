// cli/impl/log_generator_cli.cpp
#include "cli/impl/log_generator_cli.hpp"
#include "cli/impl/config_builder.hpp"
#include "cli/impl/config_validator.hpp"
#include "common/version.hpp"
#include <iostream>
#include <stdexcept>

namespace Cli::Impl {

    std::optional<Config> LogGeneratorCli::parse(int argc, char* argv[]) {
        // 使用程序名称初始化解析器
        Framework::ArgParser parser(argv[0]);

        // 1. 定义 Schema
        setup_options(parser);

        // 2. 执行解析 (Framework)
        auto args_opt = parser.parse(argc, argv);
        
        // 如果解析本身失败（如未识别的参数），ArgParser 内部已经打印了错误
        if (!args_opt) {
            parser.print_help();
            return std::nullopt;
        }
        const auto& args = *args_opt;

        // 3. 拦截通用命令 (Help / Version)
        // 如果处理了这些命令，返回 nullopt 表示不需要继续执行业务逻辑
        if (handle_general_commands(parser, args)) {
            return std::nullopt;
        }

        // 4. 构建与校验 (Business Logic)
        try {
            // Builder: Args -> Config (可能抛出类型转换或逻辑互斥异常)
            Config config = ConfigBuilder::build(args);
            
            // Validator: Check Config (检查数值范围等业务规则)
            ConfigValidator::validate(config);
            
            return config;

        } catch (const std::exception& e) {
            // 捕获所有业务层面的配置错误
            parser.print_error(e.what());
            parser.print_help();
            return std::nullopt;
        }
    }

    void LogGeneratorCli::setup_options(Framework::ArgParser& parser) const {
        parser.add_option("-h", "--help", "Display this help message.");
        parser.add_option("-v", "--version", "Display version information.");
        
        // 业务参数
        parser.add_option("-y", "--year", "Generate data for a single year.", true);
        parser.add_option("-s", "--start", "Starting year for a range.", true);
        parser.add_option("-e", "--end", "Ending year for a range.", true);
        parser.add_option("-i", "--items", "Log items per day (default: 10).", true);
        parser.add_option("-n", "--nosleep", "Enable 'no sleep' days.");
    }

    bool LogGeneratorCli::handle_general_commands(const Framework::ArgParser& parser, 
                                                  const Framework::ParsedArgs& args) const {
        if (args.has_flag("--help")) {
            parser.print_help();
            return true;
        }
        if (args.has_flag("--version")) {
            std::cout << "Version: " << AppVersion::APP_VERSION << "\n";
            std::cout << "Last Update: " << AppVersion::LAST_UPDATE << "\n";
            return true;
        }
        return false;
    }
}