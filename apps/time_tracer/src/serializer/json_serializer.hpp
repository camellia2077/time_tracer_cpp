// serializer/json_serializer.hpp
#ifndef SERIALIZER_JSON_SERIALIZER_HPP_
#define SERIALIZER_JSON_SERIALIZER_HPP_

#include "application/interfaces/i_log_serializer.hpp" // 实现 Core 接口
#include <string>
#include <vector>

namespace serializer {

/**
 * @brief JSON 序列化实现类
 * 实现�?Core 定义�?ILogSerializer 接口
 */
class JsonSerializer : public core::interfaces::ILogSerializer {
public:
  // --- 实现接口方法 ---
  std::string Serialize(const std::vector<DailyLog> &logs) override;
  std::vector<DailyLog> Deserialize(const std::string &content) override;

  // --- [修复] 补全辅助方法声明 ---
  std::string serializeDay(const DailyLog &day);
  std::string serializeDays(const std::vector<DailyLog> &days); // 之前漏了这个

  DailyLog deserializeDay(const std::string &json_content);
  std::vector<DailyLog>
  deserializeDays(const std::string &json_content); // 之前漏了这个
};

} // namespace serializer

#endif // SERIALIZER_JSON_SERIALIZER_HPP_
