// config/validator/reports/strategies/range/range_strategy.hpp
#ifndef CONFIG_VALIDATOR_REPORTS_STRATEGIES_RANGE_RANGE_STRATEGY_HPP_
#define CONFIG_VALIDATOR_REPORTS_STRATEGIES_RANGE_RANGE_STRATEGY_HPP_

#include "config/validator/reports/strategies/base_strategy.hpp"
#include <string>
#include <toml++/toml.hpp>

namespace ConfigValidator {

class RangeStrategy : public BaseStrategy {
protected:
    // [修复] 覆盖 validate_specific_keys 而不是 validate
    // [修复] 签名必须与 BaseStrategy 中定义的一致
    bool validate_specific_keys(const toml::table& query_config, const std::string& file_name) const override;
};

} // namespace ConfigValidator

#endif // CONFIG_VALIDATOR_REPORTS_STRATEGIES_RANGE_RANGE_STRATEGY_HPP_