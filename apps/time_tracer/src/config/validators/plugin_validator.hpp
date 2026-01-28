// config/validators/plugin_validator.hpp
// 移动�? validator/plugins/facade/plugin_validator.hpp
#ifndef CONFIG_VALIDATORS_PLUGIN_VALIDATOR_HPP_
#define CONFIG_VALIDATORS_PLUGIN_VALIDATOR_HPP_

#include "core/domain/ports/i_file_system.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class PluginValidator {
public:
  bool validate(core::interfaces::IFileSystem &fs, const fs::path &plugins_path,
                const std::vector<std::string> &expected_plugins) const;
};

#endif // CONFIG_VALIDATORS_PLUGIN_VALIDATOR_HPP_
