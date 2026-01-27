// reports/daily/formatters/typst/day_typ_formatter.cpp
#include "reports/daily/formatters/typst/day_typ_formatter.hpp"
#include "reports/daily/formatters/statistics/stat_formatter.hpp"
#include "reports/daily/formatters/statistics/typst_strategy.hpp"
#include "reports/core/utils/report_string_utils.hpp"
#include "reports/core/utils/report_time_format.hpp"
#include <format>

// ==========================================
// 逻辑块 A: DayTypConfig 实现
// ==========================================
DayTypConfig::DayTypConfig(const toml::table& config)
    : DayBaseConfig(config), style_(config) 
{
    statistic_font_size_ = config_table_["statistic_font_size"].value_or(10);
    statistic_title_font_size_ = config_table_["statistic_title_font_size"].value_or(12);
    
    if (const toml::table* kw_tbl = config_table_["keyword_colors"].as_table()) {
        for (const auto& [key, val] : *kw_tbl) {
            if (auto s = val.value<std::string>()) {
                keyword_colors_[std::string(key.str())] = *s;
            }
        }
    }
}

// ==========================================
// 逻辑块 B: DayTypFormatter 实现
// ==========================================
DayTypFormatter::DayTypFormatter(std::shared_ptr<DayTypConfig> config) 
    : BaseTypFormatter(config) {}

bool DayTypFormatter::is_empty_data(const DailyReportData& data) const {
    return data.total_duration == 0;
}

int DayTypFormatter::get_avg_days(const DailyReportData&) const {
    return 1;
}

std::string DayTypFormatter::get_no_records_msg() const {
    return config_->get_no_records();
}

void DayTypFormatter::format_header_content(std::stringstream& ss, const DailyReportData& data) const {
    display_header(ss, data);
}

void DayTypFormatter::format_extra_content(std::stringstream& ss, const DailyReportData& data) const {
    auto strategy = std::make_unique<TypstStrategy<DayTypConfig>>(config_.get());
    StatFormatter stats_formatter(std::move(strategy));
    ss << stats_formatter.format(data, *config_); 
    display_detailed_activities(ss, data);
}

void DayTypFormatter::display_header(std::stringstream& ss, const DailyReportData& data) const {
    std::string title = std::format(
        R"(#text(font: "{}", size: {}pt)[= {} {}])",
        config_->get_title_font(), config_->get_report_title_font_size(),
        config_->get_title_prefix(), data.date
    );
    ss << title << "\n\n";
    ss << std::format("+ *{}:* {}\n", config_->get_date_label(), data.date);
    ss << std::format("+ *{}:* {}\n", config_->get_total_time_label(), time_format_duration(data.total_duration, 1));
    ss << std::format("+ *{}:* {}\n", config_->get_status_label(), bool_to_string(data.metadata.status));
    ss << std::format("+ *{}:* {}\n", config_->get_sleep_label(), bool_to_string(data.metadata.sleep));
    ss << std::format("+ *{}:* {}\n", config_->get_exercise_label(), bool_to_string(data.metadata.exercise));
    ss << std::format("+ *{}:* {}\n", config_->get_getup_time_label(), data.metadata.getup_time);

    std::string remark = format_multiline_for_list(data.metadata.remark, 2, " \\");
    ss << std::format("+ *{}:* {}\n", config_->get_remark_label(), remark);
}

void DayTypFormatter::display_detailed_activities(std::stringstream& ss, const DailyReportData& data) const {
    if (data.detailed_records.empty()) return;

    ss << std::format(R"(#text(font: "{}", size: {}pt)[= {}])", 
        config_->get_category_title_font(), config_->get_category_title_font_size(), 
        config_->get_all_activities_label()) << "\n\n";

    for (const auto& record : data.detailed_records) {
        ss << format_activity_line(record) << "\n";
    }
}

std::string DayTypFormatter::format_activity_line(const TimeRecord& record) const {
    std::string project_path = replace_all(record.project_path, "_", config_->get_activity_connector());
    std::string base_string = std::format("{} - {} ({}): {}",
        record.start_time, record.end_time,
        time_format_duration(record.duration_seconds, 1), project_path
    );

    for (const auto& [kw, color] : config_->get_keyword_colors()) {
        if (record.project_path.find(kw) != std::string::npos) {
            std::string output = std::format("+ #text(rgb(\"{}\"))[{}]", color, base_string);
            if (record.activityRemark.has_value()) {
                std::string remark = format_multiline_for_list(record.activityRemark.value(), 4, " \\");
                output += std::format("\n  + *{}:* {}", config_->get_activity_remark_label(), remark);
            }
            return output;
        }
    }

    std::string output = "+ " + base_string;
    if (record.activityRemark.has_value()) {
        std::string remark = format_multiline_for_list(record.activityRemark.value(), 4, " \\");
        output += std::format("\n  + *{}:* {}", config_->get_activity_remark_label(), remark);
    }
    return output;
}

// --- DLL 导出接口 ---
extern "C" {
    __declspec(dllexport) FormatterHandle create_formatter(const char* config_content) {
        try {
            auto config_tbl = toml::parse(config_content);
            auto config = std::make_shared<DayTypConfig>(config_tbl);
            auto formatter = new DayTypFormatter(config);
            return static_cast<FormatterHandle>(formatter);
        } catch (...) {
            return nullptr;
        }
    }

    __declspec(dllexport) void destroy_formatter(FormatterHandle handle) {
        if (handle) delete static_cast<DayTypFormatter*>(handle);
    }

    static std::string report_buffer;
    __declspec(dllexport) const char* format_report(FormatterHandle handle, const DailyReportData& data) {
        if (handle) {
            auto* formatter = static_cast<DayTypFormatter*>(handle);
            report_buffer = formatter->format_report(data);
            return report_buffer.c_str();
        }
        return "";
    }
}