// core/config/config_handler.hpp
#pragma once
#ifndef CORE_CONFIG_CONFIG_HANDLER_HPP_
#define CORE_CONFIG_CONFIG_HANDLER_HPP_

#include "common/app_context.hpp"
#include "common/config_types.hpp"
#include "io/file_system_interfaces.hpp"
#include <memory>
#include <string>

namespace Core::Config {

    class ConfigHandler {
    public:
        // 依赖注入 FileReader 接口，便于测试或更换文件源
        explicit ConfigHandler(std::shared_ptr<IFileReader> file_reader);

        /**
         * @brief 加载配置文件并与 CLI 参数合并
         * @param cli_config 命令行解析出的配置（优先级最高）
         * @param settings_path settings.toml 路径
         * @param mapping_path mapping.toml 路径
         * @return 构建完成的应用上下文
         */
        AppContext load_and_merge(const ::Config& cli_config, 
                                  const std::string& settings_path, 
                                  const std::string& mapping_path);

    private:
        std::shared_ptr<IFileReader> file_reader_;
    };

}

#endif // CORE_CONFIG_CONFIG_HANDLER_HPP_