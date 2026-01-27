// config/validator/reports/strategies/strategy_factory.hpp
#ifndef CONFIG_VALIDATOR_REPORTS_STRATEGIES_STRATEGY_FACTORY_HPP_
#define CONFIG_VALIDATOR_REPORTS_STRATEGIES_STRATEGY_FACTORY_HPP_

#include "i_query_strategy.hpp"
#include "reports/shared/types/report_format.hpp" 
#include <string>
#include <memory>

namespace ConfigValidator {

enum class ReportType { Daily, Monthly, Weekly,Periodic };

class StrategyFactory {
public:
    static std::unique_ptr<IQueryStrategy> create(ReportType type, ReportFormat format);

    // [新增] 方便 Facade 调用，自动从文件名解析
    static std::unique_ptr<IQueryStrategy> create_from_filename(const std::string& file_name);
};

} // namespace ConfigValidator

#endif