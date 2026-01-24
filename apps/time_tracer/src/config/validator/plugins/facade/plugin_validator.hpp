// config/validator/plugins/facade/plugin_validator.hpp
#ifndef CONFIG_VALIDATOR_PLUGINS_FACADE_PLUGIN_VALIDATOR_HPP_
#define CONFIG_VALIDATOR_PLUGINS_FACADE_PLUGIN_VALIDATOR_HPP_

#include <filesystem>
#include <string>
#include <vector>
#include "core/application/interfaces/i_file_system.hpp"

namespace fs = std::filesystem;

class PluginValidator {
public:
    bool validate(core::interfaces::IFileSystem& fs, const fs::path& plugins_path, const std::vector<std::string>& expected_plugins) const;
};

#endif