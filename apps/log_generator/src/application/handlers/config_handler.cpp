// application/handlers/config_handler.cpp
// 应用层：配置加载处理器实现
#include "application/handlers/config_handler.hpp"
#include "infrastructure/persistence/toml_config_parser.hpp"
#include "cli/framework/ansi_colors.hpp"
#include <stdexcept>
#include <iostream>
#include <format>

namespace application::handlers {

ConfigHandler::ConfigHandler(std::shared_ptr<domain::ports::FileReaderInterface> file_reader)
    : file_reader_(std::move(file_reader)) {}

domain::model::GenerationContext ConfigHandler::LoadAndMerge(
    const domain::model::Config& cli_config,
    const std::string& settings_path,
    const std::string& mapping_path) {
    
    // 1. 读取原始文件内容
    auto settings_content = file_reader_->ReadFile(settings_path);
    if (!settings_content) {
        throw std::runtime_error(std::format("Critical: Failed to load settings file: {}", settings_path));
    }

    auto mapping_content = file_reader_->ReadFile(mapping_path);
    if (!mapping_content) {
        throw std::runtime_error(std::format("Critical: Failed to load mapping file: {}", mapping_path));
    }

    // 2. 解析 TOML
    infrastructure::persistence::TomlConfigParser parser;
    auto parsed_data_opt = parser.Parse(*settings_content, *mapping_content);

    if (!parsed_data_opt) {
        throw std::runtime_error("Critical: Failed to parse configuration files.");
    }

    auto& parsed_data = *parsed_data_opt;

    // 3. 构建并返回 GenerationContext
    domain::model::GenerationContext context;
    context.config = cli_config;
    
    // 移动语义转移大数据结构
    context.all_activities = std::move(parsed_data.activities);
    context.remarks = std::move(parsed_data.remarks);
    context.activity_remarks = std::move(parsed_data.activity_remarks);
    context.wake_keywords = std::move(parsed_data.wake_keywords);

    // 简单校验
    if (context.all_activities.empty()) {
        std::cerr << RED_COLOR << "Warning: No activities found in mapping file." << RESET_COLOR << "\n";
    }

    return context;
}

}  // namespace application::handlers
