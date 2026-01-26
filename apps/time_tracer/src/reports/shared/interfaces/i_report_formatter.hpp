// reports/shared/interfaces/i_report_formatter.hpp
#ifndef REPORTS_SHARED_INTERFACES_I_REPORT_FORMATTER_HPP_
#define REPORTS_SHARED_INTERFACES_I_REPORT_FORMATTER_HPP_

#include <string>
#include "reports/data/model/daily_report_data.hpp"
#include "reports/data/model/range_report_data.hpp"

template<typename ReportDataType>
class IReportFormatter {
public:
    virtual ~IReportFormatter() = default;
    virtual std::string format_report(const ReportDataType& data) const = 0;
};

// C-style interface for DLLs
#ifdef __cplusplus
extern "C" {
#endif

typedef void* FormatterHandle;
typedef void* (*CreateFormatterFunc)(const char* config_content);
typedef void (*DestroyFormatterFunc)(FormatterHandle);

// [修改] 只保留 Daily 和 Range 两种函数签名
typedef const char* (*FormatReportFunc_Day)(FormatterHandle, const DailyReportData&);
typedef const char* (*FormatReportFunc_Range)(FormatterHandle, const RangeReportData&);

#ifdef __cplusplus
}
#endif

#endif // REPORTS_SHARED_INTERFACES_I_REPORT_FORMATTER_HPP_