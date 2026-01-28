// config/loaders/converter_loader.hpp
// 合并�? loader/converter_config_loader.hpp +
// loader/toml_converter_config_loader.hpp
#ifndef CONFIG_LOADERS_CONVERTER_LOADER_HPP_
#define CONFIG_LOADERS_CONVERTER_LOADER_HPP_

#include "common/config/i_config_loader.hpp"
#include "common/config/models/converter_config_models.hpp"
#include "core/domain/ports/i_file_system.hpp"
#include <filesystem>
#include <toml++/toml.hpp>

// ============================================================================
// ConverterConfigLoader - 从文件加�?Converter 配置
// ============================================================================
class ConverterConfigLoader {
public:
  static ConverterConfig
  LoadFromFile(core::interfaces::IFileSystem &fs,
               const std::filesystem::path &main_config_path);

private:
  static void MergeTomlTable(toml::table &target, const toml::table &source);
  static toml::table
  LoadMergedToml(core::interfaces::IFileSystem &fs,
                 const std::filesystem::path &main_config_path);
  static void ParseTomlToStruct(const toml::table &tbl,
                                ConverterConfig &out_config);
};

// ============================================================================
// TomlConverterConfigLoader - 基于 TOML 表的 Converter 配置加载�?
// ============================================================================
class TomlConverterConfigLoader : public IConfigLoader<ConverterConfig> {
public:
  explicit TomlConverterConfigLoader(const toml::table &config_table);
  bool load(ConverterConfig &config_object) override;

private:
  const toml::table &toml_source_;
};

#endif // CONFIG_LOADERS_CONVERTER_LOADER_HPP_
