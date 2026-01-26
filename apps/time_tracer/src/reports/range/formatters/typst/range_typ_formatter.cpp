// reports/range/formatters/typst/range_typ_formatter.cpp
#include "reports/range/formatters/typst/range_typ_formatter.hpp"
#include <format>
#include <toml++/toml.hpp>
#include "reports/shared/utils/format/time_format.hpp"

RangeTypFormatter::RangeTypFormatter(std::shared_ptr<RangeTypConfig> config) 
    : BaseTypFormatter(config) {}

std::string RangeTypFormatter::validate_data(const RangeReportData& data) const {
    if (data.start_date.empty() || data.end_date.empty()) {
        return config_->get_invalid_data_message();
    }
    return "";
}

bool RangeTypFormatter::is_empty_data(const RangeReportData& data) const {
    return data.total_duration == 0 || data.actual_active_days == 0;
}

int RangeTypFormatter::get_avg_days(const RangeReportData& data) const {
    return (data.covered_days > 0) ? data.covered_days : std::max(1, data.actual_active_days);
}

std::string RangeTypFormatter::get_no_records_msg() const {
    return config_->get_no_records_message();
}

void RangeTypFormatter::format_page_setup(std::stringstream& ss) const {
    ss << std::format(R"(#set page(margin: (top: {}cm, bottom: {}cm, left: {}cm, right: {}cm)))",
        config_->get_margin_top_cm(),
        config_->get_margin_bottom_cm(),
        config_->get_margin_left_cm(),
        config_->get_margin_right_cm()
    ) << "\n";
}

void RangeTypFormatter::format_header_content(std::stringstream& ss, const RangeReportData& data) const {
    // 1. Title
    std::string full_title = data.report_name;
    if (!config_->get_report_title_label().empty()) {
        full_title = config_->get_report_title_label() + " " + full_title;
    }

    ss << std::format(
        R"(#text(font: "{}", size: {}pt)[= {}])",
        config_->get_title_font(),
        config_->get_report_title_font_size(),
        full_title
    ) << "\n\n";

    // 2. Subtitle (Date Range)
    ss << std::format(
        R"(#text(style: "italic")[{} {} {}])",
        data.start_date,
        config_->get_date_range_separator(),
        data.end_date
    ) << "\n\n";

    // 3. Stats List
    if (data.total_duration > 0) {
        int avg_days = get_avg_days(data);
        ss << std::format("+ *{}:* {}\n", 
            config_->get_total_time_label(), 
            time_format_duration(data.total_duration, avg_days)
        );
        ss << std::format("+ *{}:* {}\n", 
            config_->get_actual_days_label(), 
            data.actual_active_days
        );
        ss << "\n";
    }
}

// --- DLL Exports ---
extern "C" {
    __declspec(dllexport) FormatterHandle create_formatter(const char* config_toml) {
        try {
            auto config_tbl = toml::parse(config_toml);
            auto typ_config = std::make_shared<RangeTypConfig>(config_tbl);
            auto formatter = new RangeTypFormatter(typ_config);
            return static_cast<FormatterHandle>(formatter);
        } catch (...) { return nullptr; }
    }

    __declspec(dllexport) void destroy_formatter(FormatterHandle handle) {
        if (handle) delete static_cast<RangeTypFormatter*>(handle);
    }

    static std::string report_buffer;
    __declspec(dllexport) const char* format_report(FormatterHandle handle, const RangeReportData& data) {
        if (handle) {
            auto* formatter = static_cast<RangeTypFormatter*>(handle);
            report_buffer = formatter->format_report(data);
            return report_buffer.c_str();
        }
        return "";
    }
}