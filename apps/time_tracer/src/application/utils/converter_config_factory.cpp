// application/utils/converter_config_factory.cpp
#include "application/utils/converter_config_factory.hpp"
#include "config/loaders/converter_loader.hpp"
#include <iostream>

namespace fs = std::filesystem;

namespace core::pipeline {

ConverterConfig
ConverterConfigFactory::Create(core::interfaces::IFileSystem &fs,
                               const fs::path &interval_config_path,
                               const AppConfig &app_config) {
  std::cout << "Loading Converter Config from: "
            << interval_config_path.string() << std::endl;

  // 1. 委托 Config 模块加载
  ConverterConfig config =
      ConverterConfigLoader::LoadFromFile(fs, interval_config_path);

  // 2. 注入运行时参数 (修复：路径指向 mapper_config_)
  for (const auto &[path_key, path_val] :
       app_config.pipeline_.initial_top_parents_) {
    config.mapper_config_.initial_top_parents_[path_key.string()] =
        path_val.string();
  }

  return config;
}

} // namespace core::pipeline
