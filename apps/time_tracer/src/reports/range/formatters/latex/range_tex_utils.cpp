// reports/range/formatters/latex/range_tex_utils.cpp
#include "reports/range/formatters/latex/range_tex_utils.hpp"
#include "reports/shared/formatters/latex/tex_common_utils.hpp"
#include "reports/shared/formatters/latex/tex_utils.hpp"
#include "reports/shared/utils/format/time_format.hpp"

namespace RangeTexUtils {

void display_summary(std::stringstream& ss, const RangeReportData& data, const std::shared_ptr<RangeTexConfig>& config) {
    // 1. Title Construction
    std::string full_title = TexUtils::escape_latex(data.report_name);
    if (!config->get_report_title_label().empty()) {
        full_title = TexUtils::escape_latex(config->get_report_title_label()) + " " + full_title;
    }
    
    // Render Title
    TexCommonUtils::render_title(ss, full_title, config->get_report_title_font_size());

    // 2. Subtitle (Date Range)
    std::string date_info = TexUtils::escape_latex(data.start_date) + 
                            " " + TexUtils::escape_latex(config->get_date_range_separator()) + " " + 
                            TexUtils::escape_latex(data.end_date);
    
    // Render as a centered italic line or small subtitle
    ss << "\\textit{" << date_info << "}\n\n";
    ss << "\\vspace{1em}\n";

    // 3. Stats
    if (data.total_duration > 0) {
        // Calculate average
        int avg_denominator = (data.covered_days > 0) ? data.covered_days : std::max(1, data.actual_active_days);

        std::vector<TexCommonUtils::SummaryItem> items = {
            {config->get_total_time_label(), TexUtils::escape_latex(time_format_duration(data.total_duration, avg_denominator))},
            {config->get_actual_days_label(), std::to_string(data.actual_active_days)}
        };

        TexCommonUtils::render_summary_list(
            ss, items, 
            config->get_list_top_sep_pt(), 
            config->get_list_item_sep_ex()
        );
    }
}

} // namespace RangeTexUtils