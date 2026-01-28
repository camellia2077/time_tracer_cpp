// core/domain/interfaces/i_log_serializer.hpp
// 日志序列化接口 - 业务概念 (从 application/interfaces 移动)
#ifndef CORE_DOMAIN_INTERFACES_I_LOG_SERIALIZER_HPP_
#define CORE_DOMAIN_INTERFACES_I_LOG_SERIALIZER_HPP_

#include "core/domain/model/daily_log.hpp"
#include <string>
#include <vector>

namespace core::domain::interfaces {

class ILogSerializer {
public:
  virtual ~ILogSerializer() = default;

  // 将 DailyLog 序列化为 JSON 字符串
  virtual std::string Serialize(const std::vector<DailyLog> &logs) const = 0;
};

} // namespace core::domain::interfaces

#endif // CORE_DOMAIN_INTERFACES_I_LOG_SERIALIZER_HPP_
