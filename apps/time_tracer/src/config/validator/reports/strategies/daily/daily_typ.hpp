// config/validator/reports/strategies/daily/daily_typ.hpp
#ifndef CONFIG_VALIDATOR_REPORTS_STRATEGIES_DAILY_DAILY_TYP_HPP_
#define CONFIG_VALIDATOR_REPORTS_STRATEGIES_DAILY_DAILY_TYP_HPP_

#include "config/validator/reports/strategies/base_strategy.hpp"

namespace ConfigValidator {
// [修复] 类名改为 DailyTypStrategy 以匹配工厂
class DailyTypStrategy : public BaseStrategy {
protected:
    bool validate_specific_keys(const toml::table& query_config, const std::string& file_name) const override;
};
} // namespace ConfigValidator

#endif