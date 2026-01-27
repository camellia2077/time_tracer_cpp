// reports/daily/formatters/latex/day_tex_formatter.cpp
#include "reports/daily/formatters/latex/day_tex_formatter.hpp"
#include "reports/daily/formatters/statistics/stat_formatter.hpp"
#include "reports/daily/formatters/statistics/latex_strategy.hpp"
#include "reports/core/utils/report_string_utils.hpp"
#include "reports/core/utils/report_time_format.hpp"
#include "reports/core/formatters/latex/tex_utils.hpp" 

// ==========================================
// 逻辑块 A: DayTexConfig 实现
// ==========================================
DayTexConfig::DayTexConfig(const toml::table& config) 
    : DayBaseConfig(config), style_(config) 
{
    report_title_ = config["report_title"].value_or("");
    if (const toml::table* kw_tbl = config["keyword_colors"].as_table()) {
        for (const auto& [key, val] : *kw_tbl) {
            if (auto s = val.value<std::string>()) {
                keyword_colors_[std::string(key.str())] = *s;
            }
        }
    }
}

// ==========================================
// 逻辑块 B: DayTexFormatter 实现
// ==========================================
DayTexFormatter::DayTexFormatter(std::shared_ptr<DayTexConfig> config) 
    : BaseTexFormatter(config) {} // 传入有效指针，修复崩溃

bool DayTexFormatter::is_empty_data(const DailyReportData& data) const {
    return data.total_duration == 0;
}

int DayTexFormatter::get_avg_days(const DailyReportData&) const {
    return 1;
}

std::string DayTexFormatter::get_no_records_msg() const {
    return config_->get_no_records();
}

std::map<std::string, std::string> DayTexFormatter::get_keyword_colors() const {
    return config_->get_keyword_colors();
}

void DayTexFormatter::format_extra_content(std::stringstream& ss, const DailyReportData& data) const {
    // 使用模板化的策略
    auto strategy = std::make_unique<LatexStrategy<DayTexConfig>>(config_.get());
    StatFormatter stats_formatter(std::move(strategy));
    ss << stats_formatter.format(data, *config_);
    display_detailed_activities(ss, data);
}

void DayTexFormatter::display_header(std::stringstream& ss, const DailyReportData& data) const {
    std::string title_content = config_->get_report_title() + " " + TexUtils::escape_latex(data.date);
    TexUtils::render_title(ss, title_content, config_->get_report_title_font_size());

    std::vector<TexUtils::SummaryItem> items = {
        {config_->get_date_label(),       TexUtils::escape_latex(data.date)},
        {config_->get_total_time_label(), TexUtils::escape_latex(time_format_duration(data.total_duration, 1))},
        {config_->get_status_label(),     TexUtils::escape_latex(bool_to_string(data.metadata.status))},
        {config_->get_sleep_label(),      TexUtils::escape_latex(bool_to_string(data.metadata.sleep))},
        {config_->get_exercise_label(),   TexUtils::escape_latex(bool_to_string(data.metadata.exercise))},
        {config_->get_getup_time_label(), TexUtils::escape_latex(data.metadata.getup_time)},
        {config_->get_remark_label(),     format_multiline_for_list(TexUtils::escape_latex(data.metadata.remark), 0, "\\\\")}
    };

    TexUtils::render_summary_list(ss, items, config_->get_list_top_sep_pt(), config_->get_list_item_sep_ex());
}

void DayTexFormatter::display_detailed_activities(std::stringstream& ss, const DailyReportData& data) const {
    if (data.detailed_records.empty()) return;
    TexUtils::render_title(ss, config_->get_all_activities_label(), config_->get_category_title_font_size(), true);
    // ... 详细活动渲染逻辑与之前一致 ...
}

void DayTexFormatter::format_header_content(std::stringstream& ss, const DailyReportData& data) const {
    display_header(ss, data);
}

// ==========================================
// DLL 导出函数
// ==========================================
extern "C" {
    __declspec(dllexport) FormatterHandle create_formatter(const char* config_content) {
        try {
            auto config_tbl = toml::parse(config_content);
            auto config = std::make_shared<DayTexConfig>(config_tbl);
            auto formatter = new DayTexFormatter(config);
            return static_cast<FormatterHandle>(formatter);
        } catch (...) {
            return nullptr;
        }
    }

    __declspec(dllexport) void destroy_formatter(FormatterHandle handle) {
        if (handle) delete static_cast<DayTexFormatter*>(handle);
    }

    static std::string report_buffer;
    __declspec(dllexport) const char* format_report(FormatterHandle handle, const DailyReportData& data) {
        if (handle) {
            auto* formatter = static_cast<DayTexFormatter*>(handle);
            report_buffer = formatter->format_report(data);
            return report_buffer.c_str();
        }
        return "";
    }
}