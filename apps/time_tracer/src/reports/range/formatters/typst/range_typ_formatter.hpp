// reports/range/formatters/typst/range_typ_formatter.hpp
#ifndef REPORTS_RANGE_FORMATTERS_TYPST_RANGE_TYP_FORMATTER_HPP_
#define REPORTS_RANGE_FORMATTERS_TYPST_RANGE_TYP_FORMATTER_HPP_

#include "reports/core/formatters/templates/base_typ_formatter.hpp"
#include "reports/range/common/range_base_config.hpp"
#include "reports/data/model/range_report_data.hpp"
#include <toml++/toml.hpp>
#include <memory>
#include <string>

/**
 * @class RangeTypFormatterConfig
 * @brief 范围报表 Typst 专用的本地配置类。
 * 补齐了 BaseTypFormatter 模板所需的全部接口。
 */
class RangeTypFormatterConfig : public RangeBaseConfig {
public:
    explicit RangeTypFormatterConfig(const toml::table& config) 
        : RangeBaseConfig(config) {
        auto typ_node = config["typst"];
        
        // 1. 标题样式
        title_font_ = typ_node["title_font"].value_or<std::string>("Noto Serif SC");
        title_font_size_ = typ_node["title_font_size"].value_or<int>(18);
        
        // 2. 基础正文样式 (BaseTypFormatter 依赖)
        base_font_ = typ_node["base_font"].value_or<std::string>("Noto Serif SC");
        base_font_size_ = typ_node["base_font_size"].value_or<int>(11);
        line_spacing_em_ = typ_node["line_spacing"].value_or<double>(0.65);
        
        // 3. 章节/分类标题样式 (BaseTypFormatter 依赖)
        category_title_font_ = typ_node["category_font"].value_or<std::string>("Noto Serif SC");
        category_title_font_size_ = typ_node["category_font_size"].value_or<int>(14);

        // 4. 页面边距
        margin_top_ = typ_node["margin_top"].value_or<double>(2.5);
        margin_bottom_ = typ_node["margin_bottom"].value_or<double>(2.5);
        margin_left_ = typ_node["margin_left"].value_or<double>(2.5);
        margin_right_ = typ_node["margin_right"].value_or<double>(2.5);
    }

    // --- 供本地 Formatter 使用的接口 ---
    const std::string& get_title_font() const { return title_font_; }
    int get_report_title_font_size() const { return title_font_size_; }
    
    // --- 供 BaseTypFormatter 模板使用的接口 ---
    const std::string& get_base_font() const { return base_font_; }
    int get_base_font_size() const { return base_font_size_; }
    double get_line_spacing_em() const { return line_spacing_em_; }
    
    const std::string& get_category_title_font() const { return category_title_font_; }
    int get_category_title_font_size() const { return category_title_font_size_; }

    // --- 页面布局接口 ---
    double get_margin_top_cm() const { return margin_top_; }
    double get_margin_bottom_cm() const { return margin_bottom_; }
    double get_margin_left_cm() const { return margin_left_; }
    double get_margin_right_cm() const { return margin_right_; }

private:
    std::string title_font_;
    int title_font_size_;
    
    std::string base_font_;
    int base_font_size_;
    double line_spacing_em_;
    
    std::string category_title_font_;
    int category_title_font_size_;

    double margin_top_;
    double margin_bottom_;
    double margin_left_;
    double margin_right_;
};

/**
 * @class RangeTypFormatter
 * @brief 负责范围报表的 Typst 格式化实现。
 */
class RangeTypFormatter : public BaseTypFormatter<RangeReportData, RangeTypFormatterConfig> {
public:
    explicit RangeTypFormatter(std::shared_ptr<RangeTypFormatterConfig> config);

protected:
    std::string validate_data(const RangeReportData& data) const override;
    bool is_empty_data(const RangeReportData& data) const override;
    int get_avg_days(const RangeReportData& data) const override;
    std::string get_no_records_msg() const override;
    
    void format_header_content(std::stringstream& ss, const RangeReportData& data) const override;
    void format_page_setup(std::stringstream& ss) const override;
};

#endif // REPORTS_RANGE_FORMATTERS_TYPST_RANGE_TYP_FORMATTER_HPP_