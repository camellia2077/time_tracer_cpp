// core/config/config_handler.cpp
#include "core/config/config_handler.hpp"
#include "config/config.hpp"               
#include "common/ansi_colors.hpp"   
#include <iostream>

namespace Core {

    std::optional<AppContext> ConfigHandler::load(const Config& config, 
                                                  const std::filesystem::path& exe_path, 
                                                  IFileReader& file_reader) {
        
        // 1. [初始化 Context]
        AppContext context;
        context.config = config; // 直接使用传入的配置

        // 2. [路径推导]
        // 使用传入的 exe_path 推导 config 目录
        std::filesystem::path exe_dir = exe_path.parent_path();
        std::filesystem::path settings_path   = exe_dir / "config" / "activities_config.toml";
        std::filesystem::path mapping_path    = exe_dir / "config" / "mapping_config.toml";

        // 3. [IO操作] 读取配置文件
        auto mapping_content_opt = file_reader.read_file(mapping_path);
        if (!mapping_content_opt) {
            std::cerr << RED_COLOR << "Critical: Failed to read mapping config from " 
                      << mapping_path << RESET_COLOR << std::endl;
            return std::nullopt;
        }

        auto settings_content_opt = file_reader.read_file(settings_path);
        if (!settings_content_opt) {
            std::cerr << RED_COLOR << "Critical: Failed to read settings config from " 
                      << settings_path << RESET_COLOR << std::endl;
            return std::nullopt;
        }

        // 4. [业务逻辑] 解析 TOML 内容
        ConfigLoader loader;
        auto toml_configs_opt = loader.load_from_content(
            *settings_content_opt, 
            *mapping_content_opt
        );

        if (!toml_configs_opt) {
            std::cerr << RED_COLOR << "程序因配置解析失败而退出。" << RESET_COLOR << std::endl;
            return std::nullopt;
        }

        // 填充 Context
        context.all_activities = toml_configs_opt->mapped_activities;
        if (context.all_activities.empty()) {
            std::cerr << RED_COLOR << "Critical: No mapped activities found." << RESET_COLOR << std::endl;
            return std::nullopt;
        }

        context.remarks = toml_configs_opt->remarks;
        context.activity_remarks = toml_configs_opt->activity_remarks;
        context.wake_keywords = toml_configs_opt->wake_keywords;

        return context;
    }

}