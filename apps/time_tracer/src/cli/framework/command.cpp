// cli/framework/command.cpp
// 合并自: command_args.cpp, command_parser.cpp
#include "cli/framework/command.hpp"
#include <algorithm>
#include <iterator>

// ============================================================================
// CommandParser Implementation
// ============================================================================

CommandParser::CommandParser(const std::vector<std::string>& args, const ParserConfig& config) 
    : raw_args_(args), config_(config) {
    if (args.size() < 2) {
        throw std::runtime_error("No command provided.");
    }
    command_ = raw_args_[1];
    parse();
}

std::string CommandParser::get_command() const {
    return command_;
}

const std::vector<std::string>& CommandParser::get_filtered_args() const {
    return filtered_args_;
}

std::string CommandParser::get_raw_arg(size_t index) const {
    if (index >= raw_args_.size()) {
        throw std::out_of_range("Argument index out of range.");
    }
    return raw_args_[index];
}

std::optional<std::string> CommandParser::get_option(const std::vector<std::string>& keys) const {
    for (const auto& key : keys) {
        auto it = std::find(raw_args_.begin(), raw_args_.end(), key);
        if (it != raw_args_.end() && std::next(it) != raw_args_.end()) {
            return *std::next(it);
        }
    }
    return std::nullopt;
}

bool CommandParser::has_flag(const std::vector<std::string>& keys) const {
    for (const auto& key : keys) {
        if (std::find(raw_args_.begin(), raw_args_.end(), key) != raw_args_.end()) {
            return true;
        }
    }
    return false;
}

void CommandParser::parse() {
    filtered_args_ = filter_global_options(raw_args_);
}

std::vector<std::string> CommandParser::filter_global_options(const std::vector<std::string>& original_args) {
    std::vector<std::string> filtered;
    
    for (size_t i = 0; i < original_args.size(); ++i) {
        const auto& arg = original_args[i];
        
        // 1. 检查是否是带值选项 (Value Options)
        if (std::find(config_.global_value_options.begin(), config_.global_value_options.end(), arg) 
            != config_.global_value_options.end()) {
            i++; // 跳过 key 和 value
            continue;
        }
        
        // 2. 检查是否是布尔选项 (Flags)
        if (std::find(config_.global_flag_options.begin(), config_.global_flag_options.end(), arg) 
            != config_.global_flag_options.end()) {
            continue; // 仅跳过 key
        }
        
        filtered.push_back(arg);
    }
    return filtered;
}

// ============================================================================
// ParsedArgs Implementation
// ============================================================================

std::string ParsedArgs::get(const std::string& name) const {
    auto it = values_.find(name);
    return (it != values_.end()) ? it->second : "";
}

bool ParsedArgs::has(const std::string& name) const {
    return values_.find(name) != values_.end();
}

int ParsedArgs::get_as_int(const std::string& name) const {
    auto it = values_.find(name);
    if (it != values_.end()) {
        try {
            return std::stoi(it->second);
        } catch (...) {
            throw std::runtime_error("Argument '" + name + "' is not a valid integer: " + it->second);
        }
    }
    throw std::runtime_error("Argument not found: " + name);
}

// ============================================================================
// CommandValidator Implementation
// ============================================================================

ParsedArgs CommandValidator::validate(const CommandParser& parser, const std::vector<ArgDef>& defs) {
    ParsedArgs result;
    auto positional_args = parser.get_filtered_args(); 
    const size_t base_index = 2; // 跳过程序名和命令名

    for (const auto& def : defs) {
        if (def.type == ArgType::Positional) {
            size_t real_index = base_index + def.position_index;
            if (real_index < positional_args.size()) {
                result.values_[def.name] = positional_args[real_index];
            } else if (def.required) {
                throw std::runtime_error("Missing required argument: <" + def.name + ">");
            }
        } 
        else if (def.type == ArgType::Option) {
            auto val = parser.get_option(def.keys);
            if (val) {
                result.values_[def.name] = *val;
            } else if (def.required) {
                throw std::runtime_error("Missing required option: " + def.keys[0]);
            } else if (!def.default_value.empty()) {
                result.values_[def.name] = def.default_value;
            }
        }
        else if (def.type == ArgType::Flag) {
            if (parser.has_flag(def.keys)) {
                result.values_[def.name] = "true";
            }
        }
    }
    return result;
}
