// config/validator/reports/strategies/base_strategy.hpp
#ifndef CONFIG_VALIDATOR_REPORTS_STRATEGIES_BASE_STRATEGY_HPP_
#define CONFIG_VALIDATOR_REPORTS_STRATEGIES_BASE_STRATEGY_HPP_

#include "i_query_strategy.hpp"
#include <toml++/toml.hpp>

namespace ConfigValidator { // <--- 新增命名空间开始

class BaseStrategy : public IQueryStrategy {
public:
    bool validate(const toml::table& query_config, const std::string& file_name) const final;
    virtual ~BaseStrategy() = default;

protected:
    virtual bool validate_specific_keys(const toml::table& query_config, const std::string& file_name) const = 0;

private:
    bool validate_common_rules(const toml::table& query_config, const std::string& file_name) const;
    bool validate_statistics_items(const toml::table& query_config, const std::string& file_name) const;
    bool is_valid_hex_color(const std::string& color_string) const;
};

} // namespace ConfigValidator <--- 新增命名空间结束

#endif
