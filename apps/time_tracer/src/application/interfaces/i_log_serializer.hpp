// application/interfaces/i_log_serializer.hpp
#ifndef APPLICATION_INTERFACES_I_LOG_SERIALIZER_HPP_
#define APPLICATION_INTERFACES_I_LOG_SERIALIZER_HPP_

#include "core/domain/model/daily_log.hpp"
#include <string>
#include <vector>

namespace core::interfaces {

/**
 * @brief 日志序列化接�?
 * Core 层定义标准，Serializer 模块负责实现�?
 */
class ILogSerializer {
public:
  virtual ~ILogSerializer() = default;

  /**
   * @brief 将日志列表序列化为字符串 (通常�?JSON)
   */
  virtual std::string Serialize(const std::vector<DailyLog> &logs) = 0;

  /**
   * @brief 将字符串反序列化为日志列�?
   */
  virtual std::vector<DailyLog> Deserialize(const std::string &content) = 0;
};

} // namespace core::interfaces

#endif // APPLICATION_INTERFACES_I_LOG_SERIALIZER_HPP_
