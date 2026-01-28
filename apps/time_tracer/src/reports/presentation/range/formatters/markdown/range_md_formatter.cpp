// reports/presentation/range/formatters/markdown/range_md_formatter.cpp
#include "reports/presentation/range/formatters/markdown/range_md_formatter.hpp"
#include "reports/shared/utils/report_time_format.hpp"
#include <algorithm>
#include <format>
#include <toml++/toml.hpp>

RangeMdFormatter::RangeMdFormatter(
    std::shared_ptr<RangeMdFormatterConfig> config)
    : BaseMdFormatter(config) {}

std::string RangeMdFormatter::validate_data(const RangeReportData &data) const {
  if (data.start_date_.empty() || data.end_date_.empty()) {
    return config_->GetInvalidDataMessage();
  }
  return "";
}

bool RangeMdFormatter::is_empty_data(const RangeReportData &data) const {
  return data.total_duration_ == 0 || data.actual_active_days_ == 0;
}

int RangeMdFormatter::get_avg_days(const RangeReportData &data) const {
  // 优先使用覆盖天数，若无则降级为活跃天数
  return (data.covered_days_ > 0) ? data.covered_days_
                                  : std::max(1, data.actual_active_days_);
}

std::string RangeMdFormatter::get_no_records_msg() const {
  return config_->GetNoRecordsMessage();
}

void RangeMdFormatter::format_header_content(
    std::stringstream &ss, const RangeReportData &data) const {
  // 1. 标题渲染
  std::string title = data.report_name_;
  if (!config_->GetReportTitleLabel().empty()) {
    title = config_->GetReportTitleLabel() + " " + title;
  }
  ss << "# " << title << "\n\n";

  // 2. 副标题: 日期范围
  ss << "**" << data.start_date_ << "** " << config_->GetDateRangeSeparator()
     << " **" << data.end_date_ << "**\n\n";

  // 3. 统计摘要
  if (data.total_duration_ > 0) {
    int avg_denominator = get_avg_days(data);

    ss << "- **" << config_->GetTotalTimeLabel()
       << "**: " << time_format_duration(data.total_duration_, avg_denominator)
       << "\n";

    ss << "- **" << config_->GetActualDaysLabel()
       << "**: " << data.actual_active_days_ << "\n";
  }
}

// --- DLL 导出接口 ---
extern "C" {
__declspec(dllexport) FormatterHandle
create_formatter(const char *config_toml) {
  try {
    auto config_tbl = toml::parse(config_toml);
    // 使用本地定义的配置类
    auto md_config = std::make_shared<RangeMdFormatterConfig>(config_tbl);
    auto formatter = new RangeMdFormatter(md_config);
    return static_cast<FormatterHandle>(formatter);
  } catch (...) {
    return nullptr;
  }
}

__declspec(dllexport) void destroy_formatter(FormatterHandle handle) {
  if (handle) {
    delete static_cast<RangeMdFormatter *>(handle);
  }
}

static std::string report_buffer;
__declspec(dllexport) const char *format_report(FormatterHandle handle,
                                                const RangeReportData &data) {
  if (handle) {
    auto *formatter = static_cast<RangeMdFormatter *>(handle);
    report_buffer = formatter->format_report(data);
    return report_buffer.c_str();
  }
  return "";
}
}
