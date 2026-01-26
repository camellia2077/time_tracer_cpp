// config/validator/reports/strategies/daily/daily_tex.hpp
#ifndef CONFIG_VALIDATOR_REPORTS_STRATEGIES_DAILY_DAILY_TEX_HPP_
#define CONFIG_VALIDATOR_REPORTS_STRATEGIES_DAILY_DAILY_TEX_HPP_

#include "config/validator/reports/strategies/base_strategy.hpp"

namespace ConfigValidator {

/**
 * @brief LaTeX 日报配置验证策略
 */
class DailyTexStrategy : public BaseStrategy {
protected:
    bool validate_specific_keys(const toml::table& query_config, const std::string& file_name) const override;
};

} // namespace ConfigValidator

#endif // CONFIG_VALIDATOR_REPORTS_STRATEGIES_DAILY_DAILY_TEX_HPP_