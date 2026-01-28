// reports/presentation/daily/formatters/markdown/day_md_formatter.cpp
#include "reports/presentation/daily/formatters/markdown/day_md_formatter.hpp"
// [删除] #include "reports/daily/formatters/markdown/day_md_config.hpp"

#include <format>
#include <iomanip>
#include <memory>
#include <toml++/toml.hpp>

#include "reports/shared/utils/report_string_utils.hpp"
#include "reports/shared/utils/report_time_format.hpp"

#include "reports/presentation/daily/formatters/statistics/markdown_stat_strategy.hpp"
#include "reports/presentation/daily/formatters/statistics/stat_formatter.hpp"

DayMdFormatter::DayMdFormatter(std::shared_ptr<DayMdConfig> config)
    : BaseMdFormatter(config) {}

bool DayMdFormatter::is_empty_data(const DailyReportData &data) const {
  return data.total_duration_ == 0;
}

int DayMdFormatter::get_avg_days(const DailyReportData & /*data*/) const {
  return 1;
}

std::string DayMdFormatter::get_no_records_msg() const {
  return config_->GetNoRecords();
}

void DayMdFormatter::format_header_content(std::stringstream &ss,
                                           const DailyReportData &data) const {
  ss << std::format("## {0} {1}\n\n", config_->GetTitlePrefix(), data.date_);
  ss << std::format("- **{0}**: {1}\n", config_->GetDateLabel(), data.date_);
  ss << std::format("- **{0}**: {1}\n", config_->GetTotalTimeLabel(),
                    time_format_duration(data.total_duration_, 1));
  ss << std::format("- **{0}**: {1}\n", config_->GetStatusLabel(),
                    bool_to_string(data.metadata_.status_));
  ss << std::format("- **{0}**: {1}\n", config_->GetSleepLabel(),
                    bool_to_string(data.metadata_.sleep_));
  ss << std::format("- **{0}**: {1}\n", config_->GetExerciseLabel(),
                    bool_to_string(data.metadata_.exercise_));
  ss << std::format("- **{0}**: {1}\n", config_->GetGetupTimeLabel(),
                    data.metadata_.getup_time_);

  std::string formatted_remark =
      format_multiline_for_list(data.metadata_.remark_, 2, "  ");
  ss << std::format("- **{0}**: {1}\n", config_->GetRemarkLabel(),
                    formatted_remark);
}

void DayMdFormatter::format_extra_content(std::stringstream &ss,
                                          const DailyReportData &data) const {
  auto strategy = std::make_unique<MarkdownStatStrategy>();
  StatFormatter stats_formatter(std::move(strategy));
  // [修改] 解引用 shared_ptr
  ss << stats_formatter.format(data, *config_);
  _display_detailed_activities(ss, data);
}

void DayMdFormatter::_display_detailed_activities(
    std::stringstream &ss, const DailyReportData &data) const {
  if (!data.detailed_records_.empty()) {
    ss << "\n## " << config_->GetAllActivitiesLabel() << "\n\n";
    for (const auto &record : data.detailed_records_) {
      std::string project_path = replace_all(record.project_path_, "_",
                                             config_->GetActivityConnector());
      ss << std::format(
          "- {0} - {1} ({2}): {3}\n", record.start_time_, record.end_time_,
          time_format_duration(record.duration_seconds_, 1), project_path);
      if (record.activity_remark_.has_value()) {
        ss << std::format("  - **{0}**: {1}\n",
                          config_->GetActivityRemarkLabel(),
                          record.activity_remark_.value());
      }
    }
    ss << "\n";
  }
}

extern "C" {
__declspec(dllexport) FormatterHandle
create_formatter(const char *config_content) {
  try {
    auto config_tbl = toml::parse(config_content);
    auto md_config = std::make_shared<DayMdConfig>(config_tbl);
    auto formatter = new DayMdFormatter(md_config);
    return static_cast<FormatterHandle>(formatter);
  } catch (...) {
    return nullptr;
  }
}

__declspec(dllexport) void destroy_formatter(FormatterHandle handle) {
  if (handle) {
    delete static_cast<DayMdFormatter *>(handle);
  }
}

static std::string report_buffer;

__declspec(dllexport) const char *format_report(FormatterHandle handle,
                                                const DailyReportData &data) {
  if (handle) {
    auto *formatter = static_cast<DayMdFormatter *>(handle);
    report_buffer = formatter->format_report(data);
    return report_buffer.c_str();
  }
  return "";
}
}
