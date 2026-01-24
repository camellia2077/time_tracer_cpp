// config/config_loader.hpp
#ifndef CONFIG_CONFIG_LOADER_HPP_
#define CONFIG_CONFIG_LOADER_HPP_

#include <string>
#include <filesystem>
#include <memory>
#include "common/config/app_config.hpp"
#include "core/application/interfaces/i_file_system.hpp"

class ConfigLoader {
public:
    /**
     * @brief 构造函数
     * @param exe_path_str 可执行文件路径
     * @param fs 文件系统接口 (依赖注入)
     */
    ConfigLoader(const std::string& exe_path_str, std::shared_ptr<core::interfaces::IFileSystem> fs);

    AppConfig load_configuration();
    std::string get_main_config_path() const;

private:
    std::filesystem::path exe_path;
    std::filesystem::path config_dir_path;
    std::filesystem::path main_config_path;
    std::shared_ptr<core::interfaces::IFileSystem> fs_; // [新增]

    const std::string CONFIG_FILE_NAME = "config.toml";
    const std::string CONFIG_DIR_NAME = "config";
};

#endif