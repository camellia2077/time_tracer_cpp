#ifndef CORE_APPLICATION_UTILS_CONVERTER_CONFIG_FACTORY_HPP_
#define CORE_APPLICATION_UTILS_CONVERTER_CONFIG_FACTORY_HPP_

#include <filesystem>
#include "common/config/app_config.hpp"
#include "common/config/models/converter_config_models.hpp"
#include "core/application/interfaces/i_file_system.hpp"

namespace core::pipeline {

class ConverterConfigFactory {
public:
    static ConverterConfig create(core::interfaces::IFileSystem& fs,
                                  const std::filesystem::path& interval_config_path, 
                                  const AppConfig& app_config);
};

} // namespace core::pipeline

#endif // CORE_APPLICATION_UTILS_CONVERTER_CONFIG_FACTORY_HPP_