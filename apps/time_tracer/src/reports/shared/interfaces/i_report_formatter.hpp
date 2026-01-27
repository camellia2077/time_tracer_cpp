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

// [重命名] 显式指明参数是 TOML 格式的配置字符串
typedef void* (*CreateFormatterFunc)(const char* toml_config_str);

typedef void (*DestroyFormatterFunc)(FormatterHandle);

typedef const char* (*FormatReportFunc_Day)(FormatterHandle, const DailyReportData&);
typedef const char* (*FormatReportFunc_Range)(FormatterHandle, const RangeReportData&);

#ifdef __cplusplus
}
#endif

#endif // REPORTS_SHARED_INTERFACES_I_REPORT_FORMATTER_HPP_