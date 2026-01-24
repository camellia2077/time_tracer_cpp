// converter/convert/core/day_stats.hpp
#ifndef CONVERTER_CONVERT_CORE_DAY_STATS_HPP_
#define CONVERTER_CONVERT_CORE_DAY_STATS_HPP_

#include "core/domain/model/daily_log.hpp"

class DayStats {
public:
    void calculate_stats(DailyLog& day);
    // 移除了私有辅助函数，因为已经转移到了 TimeUtils
};

#endif // CONVERTER_CONVERT_CORE_DAY_STATS_HPP_