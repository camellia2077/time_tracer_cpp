// core/domain/ports/i_user_notifier.hpp
#ifndef CORE_DOMAIN_PORTS_I_USER_NOTIFIER_HPP_
#define CORE_DOMAIN_PORTS_I_USER_NOTIFIER_HPP_

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

  // 普通信?(对应原来?std::cout)
  virtual void NotifyInfo(const std::string &message) = 0;

  // 成功信息 (对应原来的 kGreenColor)
  virtual void NotifySuccess(const std::string &message) = 0;

  // 警告信息 (对应原来的 kYellowColor)
  virtual void NotifyWarning(const std::string &message) = 0;

  // 错误信息 (对应原来的 std::cerr + kRedColor)
  virtual void NotifyError(const std::string &message) = 0;
};

} // namespace core::interfaces

#endif // CORE_DOMAIN_PORTS_I_USER_NOTIFIER_HPP_
