// reports/presentation/range/formatters/typst/range_typ_formatter.cpp
#include "reports/presentation/range/formatters/typst/range_typ_formatter.hpp"
#include "reports/shared/utils/report_time_format.hpp"
#include <format>
#include <toml++/toml.hpp>

RangeTypFormatter::RangeTypFormatter(
    std::shared_ptr<RangeTypFormatterConfig> config)
    : BaseTypFormatter(config) {}

std::string
RangeTypFormatter::validate_data(const RangeReportData &data) const {
  if (data.start_date_.empty() || data.end_date_.empty()) {
    return config_->GetInvalidDataMessage();
  }
  return "";
}

bool RangeTypFormatter::is_empty_data(const RangeReportData &data) const {
  return data.total_duration_ == 0 || data.actual_active_days_ == 0;
}

int RangeTypFormatter::get_avg_days(const RangeReportData &data) const {
  return (data.covered_days_ > 0) ? data.covered_days_
                                  : std::max(1, data.actual_active_days_);
}

std::string RangeTypFormatter::get_no_records_msg() const {
  return config_->GetNoRecordsMessage();
}

void RangeTypFormatter::format_page_setup(std::stringstream &ss) const {
  ss << std::format(
            R"(#set page(margin: (top: {}cm, bottom: {}cm, left: {}cm, right: {}cm)))",
            config_->GetMarginTopCm(), config_->GetMarginBottomCm(),
            config_->GetMarginLeftCm(), config_->GetMarginRightCm())
     << "\n";
}

void RangeTypFormatter::format_header_content(
    std::stringstream &ss, const RangeReportData &data) const {
  // 1. Title
  std::string full_title = data.report_name_;
  if (!config_->GetReportTitleLabel().empty()) {
    full_title = config_->GetReportTitleLabel() + " " + full_title;
  }

  ss << std::format(R"(#text(font: "{}", size: {}pt)[= {}])",
                    config_->GetTitleFont(), config_->GetReportTitleFontSize(),
                    full_title)
     << "\n\n";

  // 2. Subtitle (Date Range)
  ss << std::format(R"(#text(style: "italic")[{} {} {}])", data.start_date_,
                    config_->GetDateRangeSeparator(), data.end_date_)
     << "\n\n";

  // 3. Stats List
  if (data.total_duration_ > 0) {
    int avg_days = get_avg_days(data);
    ss << std::format("+ *{}:* {}\n", config_->GetTotalTimeLabel(),
                      time_format_duration(data.total_duration_, avg_days));
    ss << std::format("+ *{}:* {}\n", config_->GetActualDaysLabel(),
                      data.actual_active_days_);
    ss << "\n";
  }
}

// --- DLL Exports ---
extern "C" {
__declspec(dllexport) FormatterHandle
create_formatter(const char *config_toml) {
  try {
    auto config_tbl = toml::parse(config_toml);
    // 修改实例化时的类名
    auto typ_config = std::make_shared<RangeTypFormatterConfig>(config_tbl);
    auto formatter = new RangeTypFormatter(typ_config);
    return static_cast<FormatterHandle>(formatter);
  } catch (...) {
    return nullptr;
  }
}

__declspec(dllexport) void destroy_formatter(FormatterHandle handle) {
  if (handle)
    delete static_cast<RangeTypFormatter *>(handle);
}

static std::string report_buffer;
__declspec(dllexport) const char *format_report(FormatterHandle handle,
                                                const RangeReportData &data) {
  if (handle) {
    auto *formatter = static_cast<RangeTypFormatter *>(handle);
    report_buffer = formatter->format_report(data);
    return report_buffer.c_str();
  }
  return "";
}
}
