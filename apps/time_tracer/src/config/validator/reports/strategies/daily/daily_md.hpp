// config/validator/reports/strategies/daily/daily_md.hpp
#ifndef CONFIG_VALIDATOR_REPORTS_STRATEGIES_DAILY_DAILY_MD_HPP_
#define CONFIG_VALIDATOR_REPORTS_STRATEGIES_DAILY_DAILY_MD_HPP_

#include "config/validator/reports/strategies/base_strategy.hpp"

namespace ConfigValidator { // <--- 1. 加入命名空间

// 2. 类名修改为 DailyMdStrategy 以匹配工厂调用
class DailyMdStrategy : public BaseStrategy {
protected:
    bool validate_specific_keys(const toml::table& query_config, const std::string& file_name) const override;
};

} // namespace ConfigValidator

#endif
