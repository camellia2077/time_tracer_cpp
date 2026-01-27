// config/validators/converter_validator.hpp
// 合并自: validator/converter/rules/*.hpp
#ifndef CONFIG_VALIDATORS_CONVERTER_VALIDATOR_HPP_
#define CONFIG_VALIDATORS_CONVERTER_VALIDATOR_HPP_

#include <string>
#include <toml++/toml.hpp>

// ============================================================================
// MainRule - 主配置验证规则
// ============================================================================
class MainRule {
public:
    bool validate(const toml::table& main_tbl, std::string& out_mappings_path, std::string& out_duration_rules_path) const;
};

// ============================================================================
// MappingRule - 映射配置验证规则
// ============================================================================
class MappingRule {
public:
    bool validate(const toml::table& mappings_tbl) const;
};

// ============================================================================
// DurationRule - 时长规则验证
// ============================================================================
class DurationRule {
public:
    bool validate(const toml::table& duration_tbl) const;
};

#endif // CONFIG_VALIDATORS_CONVERTER_VALIDATOR_HPP_
