// reports/daily/formatters/statistics/stat_formatter.hpp
#ifndef REPORTS_DAILY_FORMATTERS_STATISTICS_STAT_FORMATTER_HPP_
#define REPORTS_DAILY_FORMATTERS_STATISTICS_STAT_FORMATTER_HPP_

#include "reports/core/api/shared_api.hpp"
#include <string>
#include <memory>
#include "reports/data/model/daily_report_data.hpp"
#include "reports/daily/common/day_base_config.hpp"
#include "i_stat_strategy.hpp"

/**
 * @class StatFormatter
 * @brief (上下文) 负责生成日报的统计数据部分。
 */

DISABLE_C4251_WARNING // <-- [新增] 2. 禁用C4251 (因为有 std::unique_ptr)

class REPORTS_SHARED_API StatFormatter { 
public:

    explicit StatFormatter(std::unique_ptr<IStatStrategy> strategy);


    std::string format(const DailyReportData& data, const DayBaseConfig& config) const;

private:
    std::unique_ptr<IStatStrategy> m_strategy;
};

ENABLE_C4251_WARNING // <-- [新增] 4. 恢复C4251

#endif // REPORTS_DAILY_FORMATTERS_STATISTICS_STAT_FORMATTER_HPP_