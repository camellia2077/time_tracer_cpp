// application/handlers/config_handler.hpp
// 应用层：配置加载处理器
#pragma once
#ifndef APPLICATION_HANDLERS_CONFIG_HANDLER_H_
#define APPLICATION_HANDLERS_CONFIG_HANDLER_H_

#include "domain/model/generation_context.hpp"
#include "domain/model/config.hpp"
#include "domain/ports/file_system_interface.hpp"
#include <memory>
#include <string>

namespace application::handlers {

/**
 * @class ConfigHandler
 * @brief 配置加载与合并处理器
 * 
 * 职责：
 * 1. 从配置文件加载活动列表和备注配置
 * 2. 将 CLI 参数与文件配置合并
 * 3. 构建完整的 GenerationContext
 */
class ConfigHandler {
public:
    explicit ConfigHandler(std::shared_ptr<domain::ports::FileReaderInterface> file_reader);

    /**
     * @brief 加载配置文件并与 CLI 参数合并
     * @param cli_config 命令行解析出的配置（优先级最高）
     * @param settings_path activities 配置文件路径
     * @param mapping_path mapping 配置文件路径
     * @return 构建完成的生成上下文
     */
    domain::model::GenerationContext LoadAndMerge(
        const domain::model::Config& cli_config, 
        const std::string& settings_path, 
        const std::string& mapping_path
    );

private:
    std::shared_ptr<domain::ports::FileReaderInterface> file_reader_;
};

}  // namespace application::handlers

#endif  // APPLICATION_HANDLERS_CONFIG_HANDLER_H_
