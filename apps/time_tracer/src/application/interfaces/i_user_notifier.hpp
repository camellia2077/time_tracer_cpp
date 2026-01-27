// application/interfaces/i_user_notifier.hpp
#ifndef APPLICATION_INTERFACES_I_USER_NOTIFIER_HPP_
#define APPLICATION_INTERFACES_I_USER_NOTIFIER_HPP_

#include <string>

namespace core::interfaces {

/**
 * @brief 用户通知接口
 * * 用于将业务层的状态、进度、错误解耦输出�?
 * * 具体实现由应用层（Application/CLI）决定（例如打印到控制台带颜色、写入日志文件等）�?
 */
class IUserNotifier {
public:
    virtual ~IUserNotifier() = default;

    // 普通信�?(对应原来�?std::cout)
    virtual void notify_info(const std::string& message) = 0;

    // 成功信息 (对应原来�?GREEN_COLOR)
    virtual void notify_success(const std::string& message) = 0;

    // 警告信息 (对应原来�?YELLOW_COLOR)
    virtual void notify_warning(const std::string& message) = 0;

    // 错误信息 (对应原来�?std::cerr + RED_COLOR)
    virtual void notify_error(const std::string& message) = 0;
};

} // namespace core::interfaces

#endif // APPLICATION_INTERFACES_I_USER_NOTIFIER_HPP_