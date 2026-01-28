// cli/framework/arg_parser.cpp
#include "cli/framework/arg_parser.hpp"
#include <iostream>

namespace cli::framework {

    ArgParser::ArgParser(std::string prog_name) 
        : formatter_(std::move(prog_name)) {}

    void ArgParser::AddOption(const std::string& short_name, const std::string& long_name, const std::string& desc, bool requires_value) {
        defined_options_.push_back({short_name, long_name, desc, requires_value});
    }

    std::optional<ParsedArgs> ArgParser::Parse(int argc, char* argv[]) {
        std::map<std::string, std::string> values;
        std::map<std::string, bool> flags;

        // 跳过 argv[0]
        for (int i = 1; i < argc; ++i) {
            std::string current_arg = argv[i];
            const Option* opt = FindOption(current_arg);

            if (!opt) {
                formatter_.PrintError("Unrecognized option: " + current_arg);
                return std::nullopt;
            }

            if (opt->requires_value) {
                if (i + 1 < argc) {
                    values[opt->long_name] = argv[++i];
                } else {
                    formatter_.PrintError("Option " + opt->long_name + " requires an argument.");
                    return std::nullopt;
                }
            } else {
                flags[opt->long_name] = true;
            }
        }

        // 构造并返回结果对象
        return ParsedArgs(std::move(values), std::move(flags));
    }

    void ArgParser::PrintHelp() const {
        formatter_.PrintHelp(defined_options_);
    }

    void ArgParser::PrintError(const std::string& msg) const {
        formatter_.PrintError(msg);
    }

    const Option* ArgParser::FindOption(const std::string& arg) const {
        for (const auto& opt : defined_options_) {
            if (opt.short_name == arg || opt.long_name == arg) {
                return &opt;
            }
        }
        return nullptr;
    }

}  // namespace cli::framework