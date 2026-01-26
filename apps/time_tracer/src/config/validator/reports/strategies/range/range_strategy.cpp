// config/validator/reports/strategies/range/range_strategy.cpp
#include "config/validator/reports/strategies/range/range_strategy.hpp"
#include <iostream>

namespace ConfigValidator {

bool RangeStrategy::validate_specific_keys(const toml::table& query_config, const std::string& file_name) const {
    bool is_valid = true;

    // 验证 RangeBaseConfig 所需的关键字段
    // 这些字段对应于 reports/range/common/range_base_config.cpp 中的加载逻辑
    
    // 检查项目细分标签是否存在
    if (!query_config.contains("project_breakdown_label")) {
        std::cerr << "Error in " << file_name << ": Missing 'project_breakdown_label'" << std::endl;
        is_valid = false;
    }

    // 检查总时长标签
    if (!query_config.contains("total_time_label")) {
        std::cerr << "Error in " << file_name << ": Missing 'total_time_label'" << std::endl;
        is_valid = false;
    }

    // 检查实际天数标签
    if (!query_config.contains("actual_days_label")) {
        std::cerr << "Error in " << file_name << ": Missing 'actual_days_label'" << std::endl;
        is_valid = false;
    }

    return is_valid;
}

} // namespace ConfigValidator