// reports/shared/config/export_style_config.cpp
#include "reports/shared/config/export_style_config.hpp"

ExportStyleConfig::ExportStyleConfig(const toml::table &tbl) {
  // --- Common Settings ---
  base_font_size_ = tbl["base_font_size"].value_or(10);
  report_title_font_size_ = tbl["report_title_font_size"].value_or(14);
  category_title_font_size_ = tbl["category_title_font_size"].value_or(12);

  // --- TeX Settings ---
  // 对应原 TexStyleConfig 逻辑
  // 移除 value_or 后面的 <std::string> 以利用左值引用推导
  tex_main_font_ = tbl["main_font"].value_or("");
  tex_cjk_main_font_ = tbl["cjk_main_font"].value_or(tex_main_font_);

  tex_margin_in_ = tbl["margin_in"].value_or(1.0);
  tex_list_top_sep_pt_ = tbl["list_top_sep_pt"].value_or(0.0);
  tex_list_item_sep_ex_ = tbl["list_item_sep_ex"].value_or(0.0);

  // --- Typst Settings ---
  // 对应原 TypstStyleConfig 逻辑
  typst_base_font_ = tbl["base_font"].value_or("");
  typst_title_font_ = tbl["title_font"].value_or(typst_base_font_);
  typst_category_title_font_ =
      tbl["category_title_font"].value_or(typst_base_font_);

  typst_line_spacing_em_ = tbl["line_spacing_em"].value_or(0.65);
}

// --- Common Getters ---
int ExportStyleConfig::get_base_font_size() const { return base_font_size_; }
int ExportStyleConfig::get_report_title_font_size() const {
  return report_title_font_size_;
}
int ExportStyleConfig::get_category_title_font_size() const {
  return category_title_font_size_;
}

// --- TeX Getters ---
const std::string &ExportStyleConfig::get_tex_main_font() const {
  return tex_main_font_;
}
const std::string &ExportStyleConfig::get_tex_cjk_main_font() const {
  return tex_cjk_main_font_;
}
double ExportStyleConfig::get_tex_margin_in() const { return tex_margin_in_; }
double ExportStyleConfig::get_tex_list_top_sep_pt() const {
  return tex_list_top_sep_pt_;
}
double ExportStyleConfig::get_tex_list_item_sep_ex() const {
  return tex_list_item_sep_ex_;
}

// --- Typst Getters ---
const std::string &ExportStyleConfig::get_typst_base_font() const {
  return typst_base_font_;
}
const std::string &ExportStyleConfig::get_typst_title_font() const {
  return typst_title_font_;
}
const std::string &ExportStyleConfig::get_typst_category_title_font() const {
  return typst_category_title_font_;
}
double ExportStyleConfig::get_typst_line_spacing_em() const {
  return typst_line_spacing_em_;
}
