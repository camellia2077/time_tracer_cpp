// cli/log_generator_cli.hpp
// CLI 层：命令行接口
#pragma once
#ifndef CLI_LOG_GENERATOR_CLI_H_
#define CLI_LOG_GENERATOR_CLI_H_

#include "domain/model/config.hpp"
#include "cli/framework/arg_parser.hpp"
#include "cli/framework/parsed_args.hpp"
#include <optional>

namespace cli {

/**
 * @class LogGeneratorCli
 * @brief 命令行参数解析器
 * 
 * 职责：
 * - 定义支持的命令行参数
 * - 解析用户输入
 * - 构建并验证 Config 对象
 */
class LogGeneratorCli {
public:
    std::optional<domain::model::Config> Parse(int argc, char* argv[]);

private:
    void SetupOptions(framework::ArgParser& parser) const;
    bool HandleGeneralCommands(const framework::ArgParser& parser, 
                               const framework::ParsedArgs& args) const;
    
    domain::model::Config BuildConfig(const framework::ParsedArgs& args) const;
    void ValidateConfig(const domain::model::Config& config) const;
};

}  // namespace cli

#endif  // CLI_LOG_GENERATOR_CLI_H_
