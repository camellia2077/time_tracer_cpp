// config/validators/validator_facade.hpp
// 合并自: validator/facade/config_facade.hpp + converter/facade/converter_facade.hpp + reports/facade/query_facade.hpp
#ifndef CONFIG_VALIDATORS_VALIDATOR_FACADE_HPP_
#define CONFIG_VALIDATORS_VALIDATOR_FACADE_HPP_

#include <string>
#include <vector>
#include <filesystem>
#include <toml++/toml.hpp>

// ============================================================================
// ConverterFacade - Converter 配置验证入口
// ============================================================================
class ConverterFacade {
public:
    bool validate(
        const toml::table& main_tbl,
        const toml::table& mappings_tbl,
        const toml::table& duration_rules_tbl
    ) const;
};

// ============================================================================
// QueryFacade - 报表查询配置验证入口
// ============================================================================
class QueryFacade {
public:
    bool validate(
        const std::vector<std::pair<std::string, toml::table>>& query_configs
    ) const;
};

// ============================================================================
// ConfigFacade - 顶层配置验证入口
// ============================================================================
class ConfigFacade {
public:
    bool validate_converter_configs(
        const toml::table& main_tbl,
        const toml::table& mappings_tbl,
        const toml::table& duration_rules_tbl
    ) const;

    bool validate_query_configs(
        const std::vector<std::pair<std::string, toml::table>>& query_configs
    ) const;

    bool validate_plugins(const std::filesystem::path& plugins_path) const;
};

#endif // CONFIG_VALIDATORS_VALIDATOR_FACADE_HPP_
