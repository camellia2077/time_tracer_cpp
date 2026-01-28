// reports/presentation/range/formatters/latex/range_tex_formatter.cpp
#include "reports/presentation/range/formatters/latex/range_tex_formatter.hpp"
#include "reports/shared/formatters/latex/tex_utils.hpp"
#include "reports/shared/utils/report_time_format.hpp"
#include <algorithm>
#include <toml++/toml.hpp>
#include <vector>

RangeTexFormatter::RangeTexFormatter(
    std::shared_ptr<RangeTexFormatterConfig> config)
    : BaseTexFormatter(config) {}

std::string
RangeTexFormatter::validate_data(const RangeReportData &data) const {
  if (data.start_date_.empty() || data.end_date_.empty()) {
    return config_->GetInvalidDataMessage() + "\n";
  }
  return "";
}

bool RangeTexFormatter::is_empty_data(const RangeReportData &data) const {
  return data.total_duration_ == 0 || data.actual_active_days_ == 0;
}

int RangeTexFormatter::get_avg_days(const RangeReportData &data) const {
  return (data.covered_days_ > 0) ? data.covered_days_
                                  : std::max(1, data.actual_active_days_);
}

std::string RangeTexFormatter::get_no_records_msg() const {
  return config_->GetNoRecordsMessage();
}

void RangeTexFormatter::format_header_content(
    std::stringstream &ss, const RangeReportData &data) const {
  display_summary(ss, data);
}

void RangeTexFormatter::display_summary(std::stringstream &ss,
                                        const RangeReportData &data) const {
  // 1. 标题渲染
  std::string full_title = TexUtils::escape_latex(data.report_name_);
  if (!config_->GetReportTitleLabel().empty()) {
    full_title = TexUtils::escape_latex(config_->GetReportTitleLabel()) + " " +
                 full_title;
  }
  TexUtils::render_title(ss, full_title, config_->GetReportTitleFontSize());

  // 2. 副标题 (日期范围)
  std::string date_info =
      TexUtils::escape_latex(data.start_date_) + " " +
      TexUtils::escape_latex(config_->GetDateRangeSeparator()) + " " +
      TexUtils::escape_latex(data.end_date_);

  ss << "\\textit{" << date_info << "}\n\n";
  ss << "\\vspace{1em}\n";

  // 3. 统计概览
  if (data.total_duration_ > 0) {
    int avg_denominator = get_avg_days(data);

    std::vector<TexUtils::SummaryItem> items = {
        {config_->GetTotalTimeLabel(),
         TexUtils::escape_latex(
             time_format_duration(data.total_duration_, avg_denominator))},
        {config_->GetActualDaysLabel(),
         std::to_string(data.actual_active_days_)}};

    TexUtils::render_summary_list(ss, items, config_->GetListTopSepPt(),
                                  config_->GetListItemSepEx());
  }
}

// --- DLL 导出接口 ---
extern "C" {
__declspec(dllexport) FormatterHandle
create_formatter(const char *config_toml) {
  try {
    auto config_tbl = toml::parse(config_toml);
    auto tex_config = std::make_shared<RangeTexFormatterConfig>(config_tbl);
    auto formatter = new RangeTexFormatter(tex_config);
    return static_cast<FormatterHandle>(formatter);
  } catch (...) {
    return nullptr;
  }
}

__declspec(dllexport) void destroy_formatter(FormatterHandle handle) {
  if (handle)
    delete static_cast<RangeTexFormatter *>(handle);
}

static std::string report_buffer;
__declspec(dllexport) const char *format_report(FormatterHandle handle,
                                                const RangeReportData &data) {
  if (handle) {
    auto *formatter = static_cast<RangeTexFormatter *>(handle);
    report_buffer = formatter->format_report(data);
    return report_buffer.c_str();
  }
  return "";
}
}
