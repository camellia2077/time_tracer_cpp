// common/config/models/report_config_models.hpp
#ifndef COMMON_CONFIG_MODELS_REPORT_CONFIG_MODELS_HPP_
#define COMMON_CONFIG_MODELS_REPORT_CONFIG_MODELS_HPP_

#include "common/config/models/report_label_models.hpp"
#include "common/config/models/report_base_models.hpp"

struct DailyReportConfig {
    DailyReportLabels labels;
    FontConfig fonts;
    LayoutConfig layout;
};

// [新增] 统一的 Range 配置模型
struct RangeReportConfig {
    RangeReportLabels labels;
    FontConfig fonts;
    LayoutConfig layout;
};

#endif // COMMON_CONFIG_MODELS_REPORT_CONFIG_MODELS_HPP_