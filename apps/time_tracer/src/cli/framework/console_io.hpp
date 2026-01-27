// cli/framework/console_io.hpp
// 合并�? console_input.hpp, console_notifier.hpp
#ifndef CLI_FRAMEWORK_CONSOLE_IO_HPP_
#define CLI_FRAMEWORK_CONSOLE_IO_HPP_

#include <string>
#include <vector>
#include <iostream>
#include "application/interfaces/i_user_notifier.hpp"

// ============================================================================
// ConsoleInput - 处理 CLI 层面的用户输入交�?
// ============================================================================

/**
 * @class ConsoleInput
 * @brief 负责处理 CLI 层面的用户输入交互�?
 */
class ConsoleInput {
public:
    /**
     * @brief 提示用户输入文件路径（支持多个，空格分隔）�?
     * @param message 提示信息�?
     * @return 用户输入的路径列表�?
     */
    std::vector<std::string> prompt_for_paths(const std::string& message = "Enter .json file(s) or directory path(s) to process: ") const;

    /**
     * @brief 提示用户进行确认 (y/n)�?
     * @param message 提示信息�?
     * @return 如果用户输入 y �?Y 返回 true，否则返�?false�?
     */
    bool prompt_confirmation(const std::string& message = "Are you sure you want to continue?") const;
};

// ============================================================================
// ConsoleNotifier - 负责控制台输出和颜色控制
// ============================================================================

/**
 * @class ConsoleNotifier
 * @brief 负责具体的控制台打印和颜色控�?
 */
class ConsoleNotifier : public core::interfaces::IUserNotifier {
public:
    void notify_info(const std::string& message) override;
    void notify_success(const std::string& message) override;
    void notify_warning(const std::string& message) override;
    void notify_error(const std::string& message) override;
};

#endif // CLI_FRAMEWORK_CONSOLE_IO_HPP_
