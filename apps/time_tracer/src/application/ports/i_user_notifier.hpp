// application/ports/i_user_notifier.hpp
// 用户通知接口 - 技术端口（支撑接口�?
#ifndef APPLICATION_PORTS_I_USER_NOTIFIER_HPP_
#define APPLICATION_PORTS_I_USER_NOTIFIER_HPP_

#include <string>

namespace core::ports {

/**
 * @brief 用户通知接口
 * 用于将业务层的状态、进度、错误解耦输出�?
 * 具体实现由基础设施层决定（例如打印到控制台、写入日志文件等）�?
 */
class IUserNotifier {
public:
    virtual ~IUserNotifier() = default;

    // 普通信�?
    virtual void notify_info(const std::string& message) = 0;

    // 成功信息
    virtual void notify_success(const std::string& message) = 0;

    // 警告信息
    virtual void notify_warning(const std::string& message) = 0;

    // 错误信息
    virtual void notify_error(const std::string& message) = 0;
};

} // namespace core::ports

#endif // APPLICATION_PORTS_I_USER_NOTIFIER_HPP_
