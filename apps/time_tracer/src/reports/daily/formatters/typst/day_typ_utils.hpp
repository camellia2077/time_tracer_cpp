// reports/daily/formatters/typst/day_typ_utils.hpp
#ifndef REPORTS_DAILY_FORMATTERS_TYPST_DAY_TYP_UTILS_HPP_
#define REPORTS_DAILY_FORMATTERS_TYPST_DAY_TYP_UTILS_HPP_

#include <sstream>
#include <memory>
#include "reports/data/model/daily_report_data.hpp"

// [核心修复] 前置声明 DayTypConfig，解决 'DayTypConfig was not declared' 错误
class DayTypConfig;

namespace DayTypUtils {

    void display_header(std::stringstream& ss, const DailyReportData& data, const std::shared_ptr<DayTypConfig>& config);
    
    void display_detailed_activities(std::stringstream& ss, const DailyReportData& data, const std::shared_ptr<DayTypConfig>& config);

} // namespace DayTypUtils

#endif // REPORTS_DAILY_FORMATTERS_TYPST_DAY_TYP_UTILS_HPP_