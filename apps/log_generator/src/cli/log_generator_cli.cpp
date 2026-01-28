// cli/log_generator_cli.cpp
// CLI 层：命令行接口实现
#include "cli/log_generator_cli.hpp"
#include "cli/framework/ansi_colors.hpp"
#include <iostream>
#include <stdexcept>

namespace cli {

namespace {
    const std::string kAppVersion = "0.1.0.3";
    const std::string kLastUpdate = "2026-01-21";
}

std::optional<domain::model::Config> LogGeneratorCli::Parse(int argc, char* argv[]) {
    framework::ArgParser parser(argv[0]);

    // 1. 定义 Schema
    SetupOptions(parser);

    // 2. 执行解析
    auto args_opt = parser.Parse(argc, argv);
    
    if (!args_opt) {
        parser.PrintHelp();
        return std::nullopt;
    }
    const auto& args = *args_opt;

    // 3. 拦截通用命令
    if (HandleGeneralCommands(parser, args)) {
        return std::nullopt;
    }

    // 4. 构建与校验
    try {
        domain::model::Config config = BuildConfig(args);
        ValidateConfig(config);
        return config;
    } catch (const std::exception& e) {
        parser.PrintError(e.what());
        parser.PrintHelp();
        return std::nullopt;
    }
}

void LogGeneratorCli::SetupOptions(framework::ArgParser& parser) const {
    parser.AddOption("-h", "--help", "Display this help message.");
    parser.AddOption("-v", "--version", "Display version information.");
    
    parser.AddOption("-y", "--year", "Generate data for a single year.", true);
    parser.AddOption("-s", "--start", "Starting year for a range.", true);
    parser.AddOption("-e", "--end", "Ending year for a range.", true);
    parser.AddOption("-i", "--items", "Log items per day (default: 10).", true);
    parser.AddOption("-n", "--nosleep", "Enable 'no sleep' days.");
}

bool LogGeneratorCli::HandleGeneralCommands(
    const framework::ArgParser& parser, 
    const framework::ParsedArgs& args) const {
    
    if (args.HasFlag("--help")) {
        parser.PrintHelp();
        return true;
    }
    if (args.HasFlag("--version")) {
        std::cout << "Version: " << kAppVersion << "\n";
        std::cout << "Last Update: " << kLastUpdate << "\n";
        return true;
    }
    return false;
}

domain::model::Config LogGeneratorCli::BuildConfig(const framework::ParsedArgs& args) const {
    domain::model::Config config;

    config.items_per_day = 10;
    config.enable_nosleep = false;
    config.start_year = 0;
    config.end_year = 0;

    if (auto val = args.GetValue("--items")) {
        config.items_per_day = std::stoi(*val);
    }

    if (args.HasFlag("--nosleep")) {
        config.enable_nosleep = true;
    }

    auto single_year = args.GetValue("--year");
    auto start_year = args.GetValue("--start");
    auto end_year = args.GetValue("--end");

    if (single_year) {
        config.mode = domain::model::GenerationMode::kSingleYear;
        config.start_year = std::stoi(*single_year);
        config.end_year = std::stoi(*single_year);
        
        if (start_year || end_year) {
            throw std::logic_error("Cannot use --year with --start/--end.");
        }
    } else if (start_year && end_year) {
        config.mode = domain::model::GenerationMode::kYearRange;
        config.start_year = std::stoi(*start_year);
        config.end_year = std::stoi(*end_year);
    } else {
        throw std::logic_error("Must specify --year OR (--start and --end).");
    }

    return config;
}

void LogGeneratorCli::ValidateConfig(const domain::model::Config& config) const {
    if (config.start_year <= 0 || config.end_year <= 0) {
        throw std::logic_error("Years must be positive.");
    }
    if (config.end_year < config.start_year) {
        throw std::logic_error("End year cannot be earlier than start year.");
    }
    if (config.items_per_day < 2) {
        throw std::logic_error("Items per day must be >= 2.");
    }
}

}  // namespace cli
