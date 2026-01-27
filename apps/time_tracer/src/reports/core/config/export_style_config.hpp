// reports/core/config/export_style_config.hpp
#ifndef REPORTS_CORE_CONFIG_EXPORT_STYLE_CONFIG_HPP_
#define REPORTS_CORE_CONFIG_EXPORT_STYLE_CONFIG_HPP_

#include <string>
#include <toml++/toml.hpp>
#include "reports/core/api/shared_api.hpp"

DISABLE_C4251_WARNING

class REPORTS_SHARED_API ExportStyleConfig {
public:
    explicit ExportStyleConfig(const toml::table& config);

    // --- Common (通用) ---
    int get_base_font_size() const;
    int get_report_title_font_size() const;
    int get_category_title_font_size() const;

    // --- TeX Specific (TeX 专用) ---
    const std::string& get_tex_main_font() const;
    const std::string& get_tex_cjk_main_font() const;
    double get_tex_margin_in() const;
    double get_tex_list_top_sep_pt() const;
    double get_tex_list_item_sep_ex() const;

    // --- Typst Specific (Typst 专用) ---
    const std::string& get_typst_base_font() const;
    const std::string& get_typst_title_font() const;
    const std::string& get_typst_category_title_font() const;
    double get_typst_line_spacing_em() const;

private:
    // Common
    int base_font_size_;
    int report_title_font_size_;
    int category_title_font_size_;

    // TeX
    std::string tex_main_font_;     // 对应 TOML: main_font
    std::string tex_cjk_main_font_; // 对应 TOML: cjk_main_font
    double tex_margin_in_;          // 对应 TOML: margin_in
    double tex_list_top_sep_pt_;    // 对应 TOML: list_top_sep_pt
    double tex_list_item_sep_ex_;   // 对应 TOML: list_item_sep_ex

    // Typst
    std::string typst_base_font_;           // 对应 TOML: base_font
    std::string typst_title_font_;          // 对应 TOML: title_font
    std::string typst_category_title_font_; // 对应 TOML: category_title_font
    double typst_line_spacing_em_;          // 对应 TOML: line_spacing_em
};

ENABLE_C4251_WARNING

#endif // REPORTS_CORE_CONFIG_EXPORT_STYLE_CONFIG_HPP_