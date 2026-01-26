// config/validator/reports/strategies/i_query_strategy.hpp
#ifndef CONFIG_VALIDATOR_REPORTS_STRATEGIES_I_QUERY_STRATEGY_HPP_
#define CONFIG_VALIDATOR_REPORTS_STRATEGIES_I_QUERY_STRATEGY_HPP_

#include <toml++/toml.hpp>
#include <string>

namespace ConfigValidator { // <--- 新增命名空间开始

class IQueryStrategy {
public:
    virtual ~IQueryStrategy() = default;
    virtual bool validate(const toml::table& query_config, const std::string& file_name) const = 0;
};

} // namespace ConfigValidator <--- 新增命名空间结束

#endif
