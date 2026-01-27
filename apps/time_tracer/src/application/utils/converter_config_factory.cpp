// application/utils/converter_config_factory.cpp
#include "application/utils/converter_config_factory.hpp"
#include "config/loaders/converter_loader.hpp"
#include <iostream>

namespace fs = std::filesystem;

namespace core::pipeline {

ConverterConfig ConverterConfigFactory::create(core::interfaces::IFileSystem& fs,
                                             const fs::path& interval_config_path, 
                                             const AppConfig& app_config) {
    std::cout << "Loading Converter Config from: " << interval_config_path.string() << std::endl;

    // 1. 委托 Config 模块加载
    ConverterConfig config = ConverterConfigLoader::load_from_file(fs, interval_config_path);

    // 2. 注入运行时参�?(修复：路径指�?mapper_config)
    for (const auto& [path_key, path_val] : app_config.pipeline.initial_top_parents) {
        config.mapper_config.initial_top_parents[path_key.string()] = path_val.string();
    }

    return config;
}

} // namespace core::pipeline