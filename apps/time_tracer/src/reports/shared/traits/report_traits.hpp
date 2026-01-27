// reports/shared/traits/report_traits.hpp
#ifndef REPORTS_SHARED_TRAITS_REPORT_TRAITS_HPP_
#define REPORTS_SHARED_TRAITS_REPORT_TRAITS_HPP_

#include "reports/data/model/daily_report_data.hpp"
#include "reports/data/model/range_report_data.hpp"
#include "common/config/global_report_config.hpp" // 包含 DailyReportConfig 和 RangeReportConfig 定义

// 基础模板声明
template<typename DataT> 
struct ReportTraits;

// 特化: DailyReportData -> DailyReportConfig
template<> 
struct ReportTraits<DailyReportData> { 
    using ConfigType = DailyReportConfig; 
};

// 特化: RangeReportData -> RangeReportConfig
template<> 
struct ReportTraits<RangeReportData> { 
    using ConfigType = RangeReportConfig; 
};

#endif // REPORTS_SHARED_TRAITS_REPORT_TRAITS_HPP_
