// application/utils/converter_config_factory.hpp
#ifndef APPLICATION_UTILS_CONVERTER_CONFIG_FACTORY_HPP_
#define APPLICATION_UTILS_CONVERTER_CONFIG_FACTORY_HPP_

#include "common/config/app_config.hpp"
#include "common/config/models/converter_config_models.hpp"
#include "core/domain/ports/i_file_system.hpp"
#include <filesystem>

namespace core::pipeline {

class ConverterConfigFactory {
public:
  static ConverterConfig
  Create(core::interfaces::IFileSystem &fs,
         const std::filesystem::path &interval_config_path,
         const AppConfig &app_config);
};

} // namespace core::pipeline

#endif // APPLICATION_UTILS_CONVERTER_CONFIG_FACTORY_HPP_
