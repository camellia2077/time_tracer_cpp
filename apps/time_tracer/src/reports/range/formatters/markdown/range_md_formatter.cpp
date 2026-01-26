// reports/range/formatters/markdown/range_md_formatter.cpp
#include "reports/range/formatters/markdown/range_md_formatter.hpp"
#include <format>
#include <toml++/toml.hpp>
#include "reports/shared/utils/format/time_format.hpp"

RangeMdFormatter::RangeMdFormatter(std::shared_ptr<RangeMdConfig> config) 
    : BaseMdFormatter(config) {}

std::string RangeMdFormatter::validate_data(const RangeReportData& data) const {
    // 简单的有效性检查，RangeReportData 通常由 Service 保证基本有效
    if (data.start_date.empty() || data.end_date.empty()) {
        return config_->get_invalid_data_message();
    }
    return "";
}

bool RangeMdFormatter::is_empty_data(const RangeReportData& data) const {
    // 只要没有时长，或者活跃天数为0，就视为无数据
    return data.total_duration == 0 || data.actual_active_days == 0;
}

int RangeMdFormatter::get_avg_days(const RangeReportData& data) const {
    // 对于范围报告，使用 covered_days (自然天数) 还是 actual_active_days (活跃天数) 计算平均值？
    // 通常 UI 上显示的是 "Daily Average (based on active days)" 或者 "based on period"
    // 这里我们沿用之前的逻辑，通常使用 covered_days (例如过去7天的平均)，
    // 但如果 covered_days 未设置(0)，则降级使用 actual_active_days。
    return (data.covered_days > 0) ? data.covered_days : std::max(1, data.actual_active_days);
}

std::string RangeMdFormatter::get_no_records_msg() const {
    return config_->get_no_records_message();
}

void RangeMdFormatter::format_header_content(std::stringstream& ss, const RangeReportData& data) const {
    // 1. 标题: "# [Prefix] ReportName"
    // 例如: "# Last 7 Days" 或 "# Monthly Report: 2025-01"
    std::string title = data.report_name;
    if (!config_->get_report_title_label().empty()) {
        title = config_->get_report_title_label() + " " + title;
    }
    ss << "# " << title << "\n\n";

    // 2. 副标题: 日期范围 "Start - End"
    ss << "**" << data.start_date << "**" 
       << config_->get_date_range_separator() 
       << "**" << data.end_date << "**\n\n";

    // 3. 统计摘要
    if (data.total_duration > 0) {
        // 计算平均值的分母
        int avg_denominator = get_avg_days(data);

        ss << "- **" << config_->get_total_time_label() << "**: " 
           << time_format_duration(data.total_duration, avg_denominator) << "\n";
           
        ss << "- **" << config_->get_actual_days_label() << "**: " 
           << data.actual_active_days << "\n";
    }
}

// --- DLL 导出接口 ---
extern "C" {
    __declspec(dllexport) FormatterHandle create_formatter(const char* config_toml) {
        try {
            auto config_tbl = toml::parse(config_toml);
            auto md_config = std::make_shared<RangeMdConfig>(config_tbl);
            auto formatter = new RangeMdFormatter(md_config);
            return static_cast<FormatterHandle>(formatter);
        } catch (...) {
            return nullptr;
        }
    }

    __declspec(dllexport) void destroy_formatter(FormatterHandle handle) {
        if (handle) {
            delete static_cast<RangeMdFormatter*>(handle);
        }
    }

    // 线程不安全的缓冲区，但在单次调用模型中通常可以接受，或者改为返回 std::string 的拷贝（需要接口配合）
    // 这里沿用旧的模式
    static std::string report_buffer;

    __declspec(dllexport) const char* format_report(FormatterHandle handle, const RangeReportData& data) {
        if (handle) {
            auto* formatter = static_cast<RangeMdFormatter*>(handle);
            report_buffer = formatter->format_report(data);
            return report_buffer.c_str();
        }
        return "";
    }
}