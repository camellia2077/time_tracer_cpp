// reports/core/formatters/latex/tex_utils.hpp
#ifndef REPORTS_CORE_FORMATTERS_LATEX_TEX_UTILS_HPP_
#define REPORTS_CORE_FORMATTERS_LATEX_TEX_UTILS_HPP_

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "reports/data/model/project_tree.hpp"
#include "reports/core/api/shared_api.hpp"

namespace TexUtils {

/**
 * @brief 用于定义摘要列表中的一项
 */
struct SummaryItem {
    std::string label;
    std::string value; // 已经转义或格式化好的 LaTeX 字符串
};

/**
 * @brief 生成 LaTeX 文档的 Preamble（前导部分）。
 */
REPORTS_SHARED_API std::string get_tex_preamble(
    const std::string& main_font,
    const std::string& cjk_main_font,
    int font_size,
    double margin_in,
    const std::map<std::string, std::string>& keyword_colors = {}
);

/**
 * @brief 生成 LaTeX 文档的 Postfix（结尾部分）。
 */
REPORTS_SHARED_API std::string get_tex_postfix();

/**
 * @brief 转义 LaTeX 特殊字符，以防止编译错误。
 */
REPORTS_SHARED_API std::string escape_latex(const std::string& input);

/**
 * @brief 渲染带有特定字体大小设置的 LaTeX 标题
 * 生成: {\fontsize{size}{size*1.2}\selectfont \section*{content}}
 * @param is_subsection 如果为 true，则使用 \subsection*，否则使用 \section*
 */
REPORTS_SHARED_API void render_title(
    std::stringstream& ss, 
    const std::string& content, 
    int font_size, 
    bool is_subsection = false
);

/**
 * @brief 渲染紧凑的摘要信息列表 (itemize 环境)
 */
REPORTS_SHARED_API void render_summary_list(
    std::stringstream& ss,
    const std::vector<SummaryItem>& items,
    double top_sep_pt,
    double item_sep_ex
);

/**
 * @brief 将项目树格式化为 LaTeX 字符串。
 */
REPORTS_SHARED_API std::string format_project_tree(
    const reporting::ProjectTree& tree,
    long long total_duration,
    int avg_days,
    int category_title_font_size,
    double list_top_sep_pt,
    double list_item_sep_ex
);

} // namespace TexUtils

#endif // REPORTS_CORE_FORMATTERS_LATEX_TEX_UTILS_HPP_