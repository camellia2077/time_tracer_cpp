// core/application/utils/converter_config_factory.hpp
#ifndef CORE_APPLICATION_UTILS_CONVERTER_CONFIG_FACTORY_HPP_
#define CORE_APPLICATION_UTILS_CONVERTER_CONFIG_FACTORY_HPP_

#include <filesystem>
#include "common/config/models/converter_config_models.hpp"
#include "common/config/app_config.hpp"
#include "core/application/interfaces/i_file_system.hpp" // [新增] 引入接口定义

namespace core::pipeline {

class ConverterConfigFactory {
public:
    // [修改] 增加 IFileSystem& 参数
    static ConverterConfig create(core::interfaces::IFileSystem& fs,
                                  const std::filesystem::path& interval_config_path, 
                                  const AppConfig& app_config);
};

} // namespace core::pipeline

#endif // CORE_APPLICATION_UTILS_CONVERTER_CONFIG_FACTORY_HPP_