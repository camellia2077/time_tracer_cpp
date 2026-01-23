// core/config/config_handler.hpp
#ifndef CORE_CONFIG_CONFIG_HANDLER_HPP_
#define CORE_CONFIG_CONFIG_HANDLER_HPP_

#include "common/app_context.hpp"
#include "io/file_system_interfaces.hpp" 
#include <optional>
#include <filesystem> // [新增] 需要 std::filesystem::path

namespace Core {

    class ConfigHandler {
    public:
        /**
         * @brief 加载业务配置（Activity Mapping 等）。
         * [修改] 签名已更新，直接接收 CLI 解析后的 Config 和执行路径。
         * * @param config 已解析的基础配置
         * @param exe_path 可执行文件的路径（用于定位配置文件）
         * @param file_reader 文件读取接口
         */
        std::optional<AppContext> load(const Config& config, 
                                       const std::filesystem::path& exe_path, 
                                       IFileReader& file_reader);
    };

}

#endif // CORE_CONFIG_CONFIG_HANDLER_HPP_