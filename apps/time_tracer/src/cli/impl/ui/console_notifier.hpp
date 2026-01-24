// cli/impl/ui/console_notifier.hpp
#ifndef CLI_IMPL_UI_CONSOLE_NOTIFIER_HPP_
#define CLI_IMPL_UI_CONSOLE_NOTIFIER_HPP_

#include "core/application/interfaces/i_user_notifier.hpp"
#include <iostream>
// 负责具体的控制台打印和颜色控制
class ConsoleNotifier : public core::interfaces::IUserNotifier {
public:
    void notify_info(const std::string& message) override;
    void notify_success(const std::string& message) override;
    void notify_warning(const std::string& message) override;
    void notify_error(const std::string& message) override;
};

#endif // CLI_IMPL_UI_CONSOLE_NOTIFIER_HPP_