// common/app_context.hpp
#ifndef COMMON_APP_CONTEXT_HPP_
#define COMMON_APP_CONTEXT_HPP_

#include "common/config_types.hpp" // [修改] 不再依赖 config/Config.h
#include <vector>
#include <string>
#include <optional>

namespace Core {

    /**
     * @brief 应用程序上下文，包含运行所需的全部配置数据。
     * 由 ConfigHandler 生成，供 WorkflowHandler 使用。
     */
    struct AppContext {
        Config config;
        std::vector<std::string> all_activities;
        std::optional<DailyRemarkConfig> remarks;
        std::optional<ActivityRemarkConfig> activity_remarks;
        std::vector<std::string> wake_keywords;
    };

}
#endif // COMMON_APP_CONTEXT_HPP_