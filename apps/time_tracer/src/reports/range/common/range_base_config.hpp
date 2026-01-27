// reports/range/common/range_base_config.hpp
#ifndef REPORTS_RANGE_COMMON_RANGE_BASE_CONFIG_HPP_
#define REPORTS_RANGE_COMMON_RANGE_BASE_CONFIG_HPP_

#include "reports/core/api/shared_api.hpp"
#include <string>
#include <toml++/toml.hpp>

DISABLE_C4251_WARNING

class REPORTS_SHARED_API RangeBaseConfig {
public:
    explicit RangeBaseConfig(const toml::table& config);
    virtual ~RangeBaseConfig() = default;

    // 报告标题前缀 (可选，例如 "Report: ")
    const std::string& get_report_title_label() const;
    
    // 日期范围分隔符 (例如 " - " 或 " to ")
    const std::string& get_date_range_separator() const;

    // 统计标签
    const std::string& get_actual_days_label() const;
    const std::string& get_total_time_label() const;
    
    // 提示信息
    const std::string& get_no_records_message() const;
    const std::string& get_invalid_data_message() const;
    
    // 章节标题
    const std::string& get_project_breakdown_label() const;

protected:
    toml::table config_table_;

private:
    void load_base_config();

    std::string report_title_label_;
    std::string date_range_separator_;
    std::string actual_days_label_;
    std::string total_time_label_;
    std::string no_records_message_;
    std::string invalid_data_message_;
    std::string project_breakdown_label_;
};

ENABLE_C4251_WARNING

#endif // REPORTS_RANGE_COMMON_RANGE_BASE_CONFIG_HPP_