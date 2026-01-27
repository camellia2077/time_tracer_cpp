// reports/daily/formatters/statistics/latex_strategy.hpp
#ifndef REPORTS_DAILY_FORMATTERS_STATISTICS_LATEX_STRATEGY_HPP_
#define REPORTS_DAILY_FORMATTERS_STATISTICS_LATEX_STRATEGY_HPP_

#include "i_stat_strategy.hpp"
#include "reports/core/formatters/latex/tex_utils.hpp"
#include <format>
#include <vector>
#include <string>

/**
 * @brief 使用模板化配置，解开与具体 Config/Formatter 类的硬耦合
 */
template <typename TConfig>
class LatexStrategy : public IStatStrategy {
public:
    // 改用原始指针，因为策略对象在渲染期间生命周期短于 Formatter
    explicit LatexStrategy(const TConfig* config) : config_(config) {}

    std::string format_header(const std::string& title) const override {
        int category_size = config_->get_category_title_font_size();
        return std::format("{{ \\fontsize{{{}}}{{{}}}\\selectfont \\subsection*{{{}}}}}\n\n",
            category_size, category_size * 1.2, title);
    }

    std::string format_main_item(const std::string& label, const std::string& value) const override {
        return std::format("    \\item \\textbf{{{}}}: {}", 
            TexUtils::escape_latex(label), TexUtils::escape_latex(value));
    }

    std::string format_sub_item(const std::string& label, const std::string& value) const override {
        return std::format("        \\item \\textbf{{{}}}: {}", 
            TexUtils::escape_latex(label), TexUtils::escape_latex(value));
    }

    std::string build_output(const std::vector<std::string>& lines) const override {
        if (lines.empty()) return "";

        std::string compact_list_options = std::format("[topsep={}pt, itemsep={}ex]",
            config_->get_list_top_sep_pt(), config_->get_list_item_sep_ex());
        
        std::string result = "\\begin{itemize}" + compact_list_options + "\n";
        bool in_sublist = false;

        for (const auto& line : lines) {
            bool is_sub_item = (line.rfind("        ", 0) == 0);
            if (is_sub_item && !in_sublist) {
                result += "    \\begin{itemize}" + compact_list_options + "\n";
                in_sublist = true;
            } else if (!is_sub_item && in_sublist) {
                result += "    \\end{itemize}\n";
                in_sublist = false;
            }
            result += line + "\n";
        }

        if (in_sublist) result += "    \\end{itemize}\n";
        result += "\\end{itemize}\n\n";
        return result;
    }

private:
    const TConfig* config_;
};

#endif