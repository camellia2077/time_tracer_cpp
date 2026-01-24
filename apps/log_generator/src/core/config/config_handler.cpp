// core/config/config_handler.cpp
#include "core/config/config_handler.hpp"
#include "config/config.hpp" // ConfigLoader
#include "common/ansi_colors.hpp"
#include <stdexcept>
#include <iostream>
#include <format>

namespace Core::Config {

    ConfigHandler::ConfigHandler(std::shared_ptr<IFileReader> file_reader)
        : file_reader_(std::move(file_reader)) {}

    AppContext ConfigHandler::load_and_merge(const ::Config& cli_config,
                                             const std::string& settings_path,
                                             const std::string& mapping_path) {
        // 1. 读取原始文件内容
        auto settings_content = file_reader_->read_file(settings_path);
        if (!settings_content) {
            throw std::runtime_error(std::format("Critical: Failed to load settings file: {}", settings_path));
        }

        auto mapping_content = file_reader_->read_file(mapping_path);
        if (!mapping_content) {
            throw std::runtime_error(std::format("Critical: Failed to load mapping file: {}", mapping_path));
        }

        // 2. 解析 TOML
        ConfigLoader loader;
        auto toml_data_opt = loader.load_from_content(*settings_content, *mapping_content);

        if (!toml_data_opt) {
            throw std::runtime_error("Critical: Failed to parse configuration files.");
        }

        TomlConfigData& toml_data = *toml_data_opt;

        // 3. 构建并返回 AppContext
        // 注意：cli_config 中的运行时参数（年份、生成数量）优先级高于或独立于配置文件
        AppContext context;
        context.config = cli_config; 
        
        // 移动语义转移大数据结构
        context.all_activities = std::move(toml_data.mapped_activities);
        context.remarks = std::move(toml_data.remarks);
        context.activity_remarks = std::move(toml_data.activity_remarks);
        context.wake_keywords = std::move(toml_data.wake_keywords);

        // 简单校验
        if (context.all_activities.empty()) {
            std::cerr << RED_COLOR << "Warning: No activities found in mapping file." << RESET_COLOR << "\n";
        }

        return context;
    }

}