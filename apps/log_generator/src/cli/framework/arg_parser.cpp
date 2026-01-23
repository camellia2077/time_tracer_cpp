// cli/framework/arg_parser.cpp
#include "cli/framework/arg_parser.hpp"
#include <iostream>

namespace Cli::Framework {

    ArgParser::ArgParser(std::string prog_name) 
        : formatter_(std::move(prog_name)) {}

    void ArgParser::add_option(const std::string& short_name, const std::string& long_name, const std::string& desc, bool requires_value) {
        defined_options_.push_back({short_name, long_name, desc, requires_value});
    }

    std::optional<ParsedArgs> ArgParser::parse(int argc, char* argv[]) {
        std::map<std::string, std::string> values;
        std::map<std::string, bool> flags;

        // 跳过 argv[0]
        for (int i = 1; i < argc; ++i) {
            std::string current_arg = argv[i];
            const Option* opt = find_option(current_arg);

            if (!opt) {
                formatter_.print_error("Unrecognized option: " + current_arg);
                return std::nullopt;
            }

            if (opt->requires_value) {
                if (i + 1 < argc) {
                    values[opt->long_name] = argv[++i];
                } else {
                    formatter_.print_error("Option " + opt->long_name + " requires an argument.");
                    return std::nullopt;
                }
            } else {
                flags[opt->long_name] = true;
            }
        }

        // 构造并返回结果对象
        return ParsedArgs(std::move(values), std::move(flags));
    }

    void ArgParser::print_help() const {
        formatter_.print_help(defined_options_);
    }

    void ArgParser::print_error(const std::string& msg) const {
        formatter_.print_error(msg);
    }

    const Option* ArgParser::find_option(const std::string& arg) const {
        for (const auto& opt : defined_options_) {
            if (opt.short_name == arg || opt.long_name == arg) {
                return &opt;
            }
        }
        return nullptr;
    }

}