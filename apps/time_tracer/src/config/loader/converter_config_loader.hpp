// config/loader/converter_config_loader.hpp
#ifndef CONFIG_LOADER_CONVERTER_CONFIG_LOADER_HPP_
#define CONFIG_LOADER_CONVERTER_CONFIG_LOADER_HPP_

#include <filesystem>
#include <toml++/toml.hpp>
#include "common/config/models/converter_config_models.hpp"
#include "core/application/interfaces/i_file_system.hpp"

class ConverterConfigLoader {
public:
    static ConverterConfig load_from_file(core::interfaces::IFileSystem& fs, const std::filesystem::path& main_config_path);

private:
    static void merge_toml_table(toml::table& target, const toml::table& source);
    static toml::table load_merged_toml(core::interfaces::IFileSystem& fs, const std::filesystem::path& main_config_path);
    static void parse_toml_to_struct(const toml::table& tbl, ConverterConfig& out_config);
};

#endif